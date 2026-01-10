#include "AbstractDao.h"
#include "AddrMgrLog.h"
#include "AddrMgrUtilities.h"
#include <algorithm>
#include <sqlite3.h>

static constexpr uint32_t SQL_BUFFER_SIZE = 512;
static constexpr const char* SQL_QUERY_COUNT = "SELECT COUNT(*) FROM %s WHERE %s;";
static constexpr const char* SQL_QUERY = "SELECT * FROM %s WHERE %s ORDER BY rid %s LIMIT ? OFFSET ?;";

namespace addrbook {

AbstractDao::AbstractDao(const std::string& table_name)
  : m_table_name{table_name}
  , m_sql_count{"SELECT COUNT(*) FROM " + table_name + ";"}
  , m_sql_delete_by_rid{"DELETE FROM " + table_name + " WHERE rid  IN (%s);"}
  , m_sql_delete_all{"DELETE FROM " + table_name + ";"}
{}

uint32_t AbstractDao::GetCount() const
{
    size_t ret = 0;
    SQLite::Statement query(GetDb(), m_sql_count);
    int32_t code = query.tryExecuteStep();
    if (SQLITE_ROW == code)
    {
        ret = static_cast<uint32_t>(query.getColumn(0).getUInt());
    }
    else
    {
        AB_LOG_E("GetCount failed, code: %d %s", code, m_sql_count.c_str());
    }
    return ret;
}

bool AbstractDao::IsExist(const std::vector<uint32_t>& rids) const
{
    bool ret = false;
    if (!rids.empty())
    {
        std::string ids = AddrMgrUtilities::JoinIds(rids);
        std::string sql = "SELECT COUNT(rid) FROM " + m_table_name + " WHERE rid IN (" + ids + ");";
        SQLite::Statement stmt(GetDb(), sql);
        int32_t code = stmt.tryExecuteStep();
        if (SQLITE_ROW == code)
        {
            uint32_t count = static_cast<uint32_t>(stmt.getColumn(0).getUInt());
            if (count == static_cast<uint32_t>(rids.size()))
            {
                ret = true;
            }
            else
            {
                AB_LOG_E("IsExist failed %u %s %s", count, sql.c_str(), ids.c_str());
            }
        }
        else
        {
            AB_LOG_E("IsExist failed, code: %d", code);
        }
    }
    return ret;
}

bool AbstractDao::Remove(const std::vector<uint32_t>& rids)
{
    std::string str_ids = AddrMgrUtilities::JoinIds(rids);
    std::array<char, SQL_BUFFER_SIZE> sql_buffer = {0};
    snprintf(sql_buffer.data(), sql_buffer.size(), m_sql_delete_by_rid.c_str(), str_ids.c_str());
    return OnExecuteSql(sql_buffer.data());
}

void AbstractDao::BindWhereParams(SQLite::Statement& stmt, const std::vector<std::string>& args, uint32_t start_idx) const
{
    for (const std::string& arg : args)
    {
        stmt.bind(start_idx++, arg);
    }
}

bool AbstractDao::OnExecuteSql(const std::string& sql, const std::vector<StmtParam>& stmt_params) const
{
    SQLite::Statement stmt(GetDb(), sql);
    bool ret = BindStmtParams(stmt, stmt_params);
    if (ret)
    {
        int32_t code = stmt.tryExecuteStep();
        if (code != SQLITE_DONE)
        {
            ret = false;
            AB_LOG_E("failed to execute sql, code:%d sql:%s", code, sql.c_str());
        }
    }
    return ret;
}

bool AbstractDao::OnExecuteSql(const std::string& sql, const std::vector<std::vector<StmtParam>>& stmt_params_vec) const
{
    bool ret = true;
    SQLite::Statement stmt(GetDb(), sql);
    for (const std::vector<StmtParam>& stmt_params : stmt_params_vec)
    {
        ret = BindStmtParams(stmt, stmt_params);
        if (ret)
        {
            int32_t code = stmt.tryExecuteStep();
            if (code != SQLITE_DONE)
            {
                ret = false;
                AB_LOG_E("failed to execute sql, code:%d sql:%s", code, sql.c_str());
            }
            stmt.tryReset();
            stmt.clearBindings();
        }
        else
        {
            ret = false;
            AB_LOG_E("failed to execute sql, sql:%s", sql.c_str());
            break;
        }
    }
    return ret;
}

bool AbstractDao::BindStmtParams(SQLite::Statement& stmt, const std::vector<StmtParam>& stmt_params)
{
    bool ret = true;
    for (uint32_t i = 0; i < stmt_params.size(); ++i)
    {
        const StmtParam& stmt_param = stmt_params[i];
        StmtParamType type = stmt_param.GetType();
        if (StmtParamType::INT32 == type)
        {
            stmt.bind(i + 1, stmt_param.GetInt32Value());
        }
        else if (StmtParamType::UINT32 == type)
        {
            stmt.bind(i + 1, stmt_param.GetUInt32Value());
        }
        else if (StmtParamType::STRING == type)
        {
            stmt.bind(i + 1, stmt_param.GetStringValue());
        }
        else
        {
            AB_LOG_E("invalid stmt param type:%d", static_cast<int32_t>(type));
            ret = false;
            break;
        }
    }
    return ret;
}

PageResult AbstractDao::DoFindByPage(const QueryParams& params)
{
    PageResult page_result(params.GetPage(), params.GetPageSize());
    const CustomWhere& conditions = params.GetConditions();
    const std::string& sql = conditions.GetSql();
    const std::string& where_sql = conditions.GetWhereSql();
    uint32_t total_records = QueryCount(where_sql, conditions);
    if (total_records != 0)
    {
        std::vector<std::shared_ptr<AbstractEntity>> records = QueryRecords(sql, where_sql, params);
        page_result.SetRecords(records);
        page_result.SetTotalRecords(total_records);
    }
    return page_result;
}

uint32_t AbstractDao::QueryCount(const std::string& where_sql, const CustomWhere& conditions)
{
    uint32_t total = 0;
    std::array<char, SQL_BUFFER_SIZE> sql_buffer = {0};
    snprintf(sql_buffer.data(), sql_buffer.size(), SQL_QUERY_COUNT, m_table_name.c_str(), where_sql.c_str());
    // 创建 Statement (此时 SQL 已经是完整的了)
    SQLite::Statement stmt(AbstractDao::GetDb(), sql_buffer.data());
    // 绑定参数 绑定 WHERE 部分的参数 (从索引 1 开始)
    const std::vector<std::string>& args = conditions.GetWhereSqlArgs();
    BindWhereParams(stmt, args, 1);
    int32_t code = stmt.tryExecuteStep();
    if (SQLITE_ROW == code)
    {
        total = static_cast<uint32_t>(stmt.getColumn(0).getUInt());
    }
    else
    {
        AB_LOG_E("GetCount failed, code: %d %s", code, sql_buffer.data());
    }
    return total;
}

std::vector<std::shared_ptr<AbstractEntity>> AbstractDao::QueryRecords(const std::string& sql, const std::string& where_sql, const QueryParams& params)
{
    const CustomWhere& conditions = params.GetConditions();
    std::array<char, SQL_BUFFER_SIZE> sql_buffer = {0};
    snprintf(sql_buffer.data(), sql_buffer.size(), sql.c_str(), m_table_name.c_str(), where_sql.c_str(), OrderType::ASC == params.GetOrderBy() ? "ASC" : "DESC");

    SQLite::Statement stmt(AbstractDao::GetDb(), sql_buffer.data());
    // 第三步：绑定参数
    // 绑定 WHERE 部分的参数 (从索引 1 开始)
    const std::vector<std::string>& args = conditions.GetWhereSqlArgs();
    BindWhereParams(stmt, args, 1);
    // 绑定分页参数 (索引接在 where 参数后面)
    uint32_t limit_idx = args.size();
    stmt.bind(++limit_idx, params.GetPageSize());
    stmt.bind(++limit_idx, CalcPageOffset(params));
    std::vector<std::shared_ptr<AbstractEntity>> records;
    int32_t code = stmt.tryExecuteStep();
    while (SQLITE_ROW == code)
    {
        std::shared_ptr<AbstractEntity> entity = OnCreateEntity(stmt);
        records.emplace_back(entity);
        code = stmt.tryExecuteStep();
    }
    if (code != SQLITE_DONE)
    {
        AB_LOG_E("Query email failed, code: %d %s", code, sql_buffer.data());
    }
    return records;
}
}  // namespace addrbook