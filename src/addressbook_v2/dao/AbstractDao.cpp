#include "AbstractDao.h"
#include "AddrCenterLog.h"
#include "AddressCenterUtilities.h"
#include <algorithm>
#include <sqlite3.h>

static constexpr uint32_t SQL_BUFFER_SIZE = 512;

AbstractDao::AbstractDao(const std::string& table_name)
  : m_table_name{table_name}
  , m_sql_count{"SELECT COUNT(*) FROM " + table_name + ";"}
  , m_sql_delete_by_rid{"DELETE FROM " + table_name + " WHERE rid  IN (%s);"}
  , m_sql_delete_all{"DELETE FROM " + table_name + ";"}
{}

size_t AbstractDao::GetCount() const
{
    size_t ret = 0;
    SQLite::Statement query(GetDb(), m_sql_count);
    int32_t code = query.tryExecuteStep();
    if (SQLITE_ROW == code)
    {
        ret = static_cast<size_t>(query.getColumn(0).getUInt());
    }
    return ret;
}

bool AbstractDao::IsExist(const std::vector<uint32_t>& rids)
{
    bool ret = false;
    if (!rids.empty())
    {
        std::string ids = AddressCenterUtilities::JoinIds(rids);
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
                AB_LOG_E("IsExist failed %u %u %s", count, rids.size(), ids.c_str());
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
    std::string str_ids = AddressCenterUtilities::JoinIds(rids);
    std::array<char, SQL_BUFFER_SIZE> sql_buffer = {0};
    snprintf(sql_buffer.data(), sql_buffer.size(), m_sql_delete_by_rid.c_str(), str_ids.c_str());
    return OnExecuteSql(sql_buffer.data());
}

// 计算分页偏移量
uint32_t AbstractDao::CalcPageOffset(const QueryParams& params) const
{
    return (params.GetPage() - 1) * params.GetPageSize();
}

// 校验分页参数
bool AbstractDao::ValidatePageParams(const QueryParams& params) const
{
    bool result = true;
    if (params.GetPage() < 1 || params.GetPageSize() < 1)
    {
        result = false;
    }
    return result;
}

// 拼接 WHERE 子句（参数化，避免 SQL 注入）
std::string AbstractDao::BuildWhereClause(const std::vector<ConditionNode>& conditions, SQLite::Statement& stmt, uint32_t& param_idx) const
{
    std::string where;
    if (conditions.empty())
    {
        where = " WHERE ";
        for (size_t i = 0; i < conditions.size(); ++i)
        {
            const ConditionNode& cond = conditions[i];
            where += cond.GetField() + " " + cond.GetOp() + " ?";
            if (i != conditions.size() - 1)
            {
                where += " AND ";
            }
            stmt.bind(++param_idx, cond.GetValue());
        }
    }
    return where;
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