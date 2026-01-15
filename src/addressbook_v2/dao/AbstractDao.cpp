#include "AbstractDao.h"
#include "AddrMgrLog.h"
#include "AddrMgrSqlDefs.h"
#include "AddrMgrUtilities.h"
#include "SQLiteConn.h"
#include <algorithm>
#include <sqlite3.h>

namespace addrbook {

SQLite::Database& AbstractDao::GetDb()
{
    static SQLiteConn db(DB_NAME);
    return db.GetDb();
}

AbstractDao::AbstractDao(const std::string& table_name)
  : m_table_name{table_name}
  , m_sql_count{"SELECT COUNT(*) FROM " + table_name + ";"}
  , m_sql_delete_by_rid{"DELETE FROM " + table_name + " WHERE rid  IN (%s);"}
  , m_sql_delete_all{"DELETE FROM " + table_name + ";"}
  , m_sql_exist{"SELECT COUNT(rid) FROM " + table_name + " WHERE rid IN (%s);"}
{}

size_t AbstractDao::GetCount() const
{
    size_t ret = 0;
    SQLite::Statement query(GetDb(), m_sql_count);
    int32_t code = query.tryExecuteStep();
    if (SQLITE_ROW == code)
    {
        ret = query.getColumn(0).getUInt();
    }
    else
    {
        AB_LOG_E("%s failed code:%d ", m_sql_count.c_str(), code);
    }
    return ret;
}

bool AbstractDao::IsExist(const std::vector<uint32_t>& rids) const
{
    bool ret = false;
    if (!rids.empty())
    {
        std::string sql = AddrMgrUtilities::ReplaceFirst(m_sql_exist, AddrMgrUtilities::Join(rids));

        SQLite::Statement stmt(GetDb(), sql);
        int32_t code = stmt.tryExecuteStep();
        if (SQLITE_ROW == code)
        {
            size_t count = stmt.getColumn(0).getUInt();
            if (count == rids.size())
            {
                ret = true;
            }
            else
            {
                AB_LOG_E("IsExist failed %u %s", count, sql.c_str());
            }
        }
        else
        {
            AB_LOG_E("%s failed code:%d", sql.c_str(), code);
        }
    }
    return ret;
}

bool AbstractDao::Remove(const std::vector<uint32_t>& rids)
{
    std::string sql = AddrMgrUtilities::ReplaceFirst(m_sql_delete_by_rid, AddrMgrUtilities::Join(rids));
    return OnExecuteSql(sql);
}

PageResult AbstractDao::DoFindByPage(const QueryParams& params)
{
    PageResult page_result(params.GetPage(), params.GetPageSize());
    const CustomWhere& conditions = params.GetConditions();
    const std::string& sql = conditions.GetSql();
    const std::string& where_sql = conditions.GetWhereSql();
    size_t total_records = QueryCount(where_sql, conditions);
    if (total_records != 0)
    {
        std::vector<std::shared_ptr<AbstractEntity>> records = QueryRecords(sql, where_sql, params);
        page_result.SetRecords(records);
        page_result.SetTotalRecords(total_records);
    }
    return page_result;
}

size_t AbstractDao::QueryCount(const std::string& where_sql, const CustomWhere& conditions)
{
    std::string sql = AddrMgrUtilities::ReplaceFirst(SQL_COMMON_QUERY_COUNT, m_table_name);
    sql = AddrMgrUtilities::ReplaceFirst(sql, where_sql);

    SQLite::Statement stmt(AbstractDao::GetDb(), sql);
    BindWhereParams(stmt, conditions.GetWhereSqlArgs(), 1);

    size_t total = 0;
    int32_t code = stmt.tryExecuteStep();
    if (SQLITE_ROW == code)
    {
        total = stmt.getColumn(0).getUInt();
    }
    else
    {
        AB_LOG_E("%s failed code:%d", sql.c_str(), code);
    }

    return total;
}

std::vector<std::shared_ptr<AbstractEntity>> AbstractDao::QueryRecords(const std::string& sql, const std::string& where_sql, const QueryParams& params)
{
    std::string sql_buffer = AddrMgrUtilities::ReplaceFirst(sql, m_table_name);
    sql_buffer = AddrMgrUtilities::ReplaceFirst(sql_buffer, where_sql);
    sql_buffer = AddrMgrUtilities::ReplaceFirst(sql_buffer, OrderType::ASC == params.GetOrderBy() ? "ASC" : "DESC");

    SQLite::Statement stmt(AbstractDao::GetDb(), sql_buffer);
    const CustomWhere& conditions = params.GetConditions();
    const std::vector<std::string>& args = conditions.GetWhereSqlArgs();
    BindWhereParams(stmt, args, 1);
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
        AB_LOG_E("%s failed code:%d", sql_buffer.c_str(), code);
    }
    return records;
}

void AbstractDao::BindWhereParams(SQLite::Statement& stmt, const std::vector<std::string>& args, uint32_t start_idx) const
{
    for (const std::string& arg : args)
    {
        stmt.bind(start_idx++, arg);
    }
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

bool AbstractDao::OnExecuteSql(const std::string& sql) const
{
    std::vector<StmtParam> stmt_params;
    return OnExecuteSql(sql, std::move(stmt_params));
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
            AB_LOG_E("%s failed code:%d", sql.c_str(), code);
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
                AB_LOG_E("%s failed code:%d", sql.c_str(), code);
            }
            stmt.tryReset();
            stmt.clearBindings();
        }
        else
        {
            ret = false;
            AB_LOG_E("%s failed", sql.c_str());
            break;
        }
    }
    return ret;
}

}  // namespace addrbook