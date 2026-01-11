#include "AddrMgrLog.h"
#include "AddrMgrSqlDefs.h"
#include "AddrMgrUtilities.h"
#include "EmailGroupDao.h"
#include <algorithm>
#include <sqlite3.h>

namespace addrbook {
EmailGroupDao::EmailGroupDao()
  : AbstractDao(SQL_EMAILGROUP_TABLE_NAME)
{
}

bool EmailGroupDao::Create()
{
    return AbstractDao::OnExecuteSql(SQL_EMAILGROUP_CREATE_TABLE);
}

bool EmailGroupDao::InsertBatch(const std::vector<EmailGroupEntity>& items)
{
    std::vector<std::vector<StmtParam>> stmt_paramss;
    std::transform(
        items.cbegin(),
        items.cend(),
        std::back_inserter(stmt_paramss),
        [](const EmailGroupEntity& item) {
            std::vector<StmtParam> stmt_params;
            stmt_params.emplace_back(item.GetMailRid());
            stmt_params.emplace_back(item.GetGroupRid());
            return stmt_params;
        });

    return AbstractDao::OnExecuteSql(SQL_EMAILGROUP_INSERT, stmt_paramss);
}

bool EmailGroupDao::HasMemberOverGroupLimit(const std::vector<uint32_t>& group_ids, uint32_t limit) const
{
    return CheckMemberLimit(group_ids, SQL_EMAILGROUP_CHECK_OVER_GROUP_LIMIT, limit);
}

bool EmailGroupDao::HasMemberOverEMailLimit(const std::vector<uint32_t>& email_ids, uint32_t limit) const
{
    return CheckMemberLimit(email_ids, SQL_EMAILGROUP_CHECK_OVER_EMAIL_LIMIT, limit);
}

bool EmailGroupDao::CheckMemberLimit(const std::vector<uint32_t>& ids, const std::string& content, uint32_t limit) const
{
    bool ret = false;

    std::string str_sql = AddrMgrUtilities::ReplaceFirst(content, AddrMgrUtilities::JoinIds(ids));

    SQLite::Statement stmt(AbstractDao::GetDb(), str_sql);
    stmt.bind(1, limit);

    int32_t code = stmt.tryExecuteStep();
    if (SQLITE_DONE == code)
    {
        ret = true;
    }
    else
    {
        AB_LOG_E("%s failed code:%d", str_sql.c_str(), code);
    }

    return ret;
}

bool EmailGroupDao::RemoveByEmailRid(uint32_t email_rid)
{
    StmtParam param(email_rid);

    return AbstractDao::OnExecuteSql(SQL_EMAILGROUP_REMOVE_BY_EMAIL_RID, {param});
}
}  // namespace addrbook