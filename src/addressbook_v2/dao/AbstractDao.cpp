#include "AbstractDao.h"
#include <sqlite3.h>

static constexpr const char* SQL_ROW_COUNT = "SELECT COUNT(*) FROM ";
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

uint32_t AbstractDao::GetCount() const
{
    uint32_t ret = 0;
    SQLite::Statement query(GetDb(), SQL_ROW_COUNT + m_table_name + ";");
    int32_t code = query.tryExecuteStep();
    if (SQLITE_ROW == code)
    {
        ret = query.getColumn(0).getUInt();
    }
    return ret;
}