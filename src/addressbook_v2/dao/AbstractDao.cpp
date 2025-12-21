#include "AbstractDao.h"
#include "AddrCenterLog.h"
#include <algorithm>
#include <sqlite3.h>

AbstractDao::AbstractDao(const std::string& table_name)
  : m_table_name{table_name}
  , m_count{"SELECT COUNT(*) FROM " + table_name + ";"}
  , m_delete_by_rid{"DELETE FROM " + table_name + " WHERE rid = ?;"}
  , m_delete_all{"DELETE FROM " + table_name + ";"}
{}

size_t AbstractDao::GetCount() const
{
    size_t ret = 0;
    SQLite::Statement query(GetDb(), m_count);
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
        // 去掉重复的
        std::vector<uint32_t> unique_rids = rids;
        std::sort(unique_rids.begin(), unique_rids.end());
        std::vector<uint32_t>::iterator last = std::unique(unique_rids.begin(), unique_rids.end());
        unique_rids.erase(last, unique_rids.end());

        std::string ids = JoinIds(unique_rids);
        std::string sql = "SELECT COUNT(rid) FROM " + m_table_name + " WHERE rid IN (" + ids + ");";
        SQLite::Statement stmt(GetDb(), sql);
        int32_t code = stmt.tryExecuteStep();
        if (SQLITE_ROW == code)
        {
            size_t count = static_cast<size_t>(stmt.getColumn(0).getUInt());
            if (count == unique_rids.size())
            {
                ret = true;
            }
            else
            {
                AB_LOG_E("IsExist failed %u %u %s", count, unique_rids.size(), ids.c_str());
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
    bool ret = true;
    SQLite::Statement stmt(GetDb(), m_delete_by_rid);
    for (uint32_t rid : rids)
    {
        stmt.bind(1, rid);
        int32_t code = stmt.tryExecuteStep();
        if (SQLITE_DONE != code)
        {
            AB_LOG_E("Remove email failed, code: %d", code);
            ret = false;
            break;
        }
        stmt.reset();
        stmt.clearBindings();
    }
    return ret;
}

bool AbstractDao::RemoveAll()
{
    return OnExecuteSql(m_delete_all);
}

/*-----------------------------------*/

// 计算分页偏移量
uint32_t AbstractDao::CalcPageOffset(const QueryParams& params) const
{
    return (params.page - 1) * params.page_size;
}

// 校验分页参数
DaoErrCode AbstractDao::ValidatePageParams(const QueryParams& params) const
{
    DaoErrCode result = DaoErrCode::SUCCESS;
    if (params.page < 1 || params.page_size < 1)
    {
        result = DaoErrCode::INVALID_PARAM;
    }
    return result;
}

// 拼接 WHERE 子句（参数化，避免 SQL 注入）
std::string AbstractDao::BuildWhereClause(const std::vector<ConditionNode>& conditions, SQLite::Statement& stmt, uint32_t& param_idx) const
{
    if (conditions.empty())
        return "";

    std::string where = " WHERE ";
    param_idx = 1;  // SQLiteCpp 参数从 1 开始
    for (size_t i = 0; i < conditions.size(); ++i)
    {
        const auto& cond = conditions[i];
        where += cond.field + " " + cond.op + " ?";  // 占位符，避免拼接值
        if (i != conditions.size() - 1)
        {
            where += " AND ";
        }
        // 绑定参数（SQLiteCpp 类型安全绑定）
        stmt.bind(param_idx++, cond.value);
    }
    return where;
}

bool AbstractDao::OnExecuteSql(const std::string& sql) const
{
    bool ret = false;
    SQLite::Statement stmt(GetDb(), sql);
    int32_t code = stmt.tryExecuteStep();
    if (SQLITE_DONE == code || SQLITE_ROW == code)
    {
        ret = true;
    }
    else
    {
        AB_LOG_E("Failed to create table, code: %d", code);
    }
    return ret;
}

std::string AbstractDao::JoinIds(const std::vector<uint32_t>& rids)
{
    if (rids.empty())
    {
        return "";
    }

    std::string result;
    // 预估大小，避免多次内存分配
    result.reserve(rids.size() * 10);

    result += std::to_string(rids[0]);
    for (size_t i = 1; i < rids.size(); ++i)
    {
        result += ",";
        result += std::to_string(rids[i]);
    }
    return result;
}