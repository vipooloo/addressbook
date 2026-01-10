#include "AddrMgrLog.h"
#include "AddrMgrUtilities.h"
#include "EmailGroupDao.h"
#include <algorithm>
#include <array>
#include <sqlite3.h>

static constexpr const char* SQL_TABLE_NAME = "GROUPMAPPING";
static constexpr const char* SQL_CREATE_TABLE = R"(
CREATE TABLE IF NOT EXISTS GROUPMAPPING (
    id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    m_rid INTEGER,  
    g_rid INTEGER,  
    FOREIGN KEY (g_rid) REFERENCES MAILGROUP (rid) ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (m_rid) REFERENCES MAIL      (rid) ON DELETE CASCADE ON UPDATE CASCADE,
    UNIQUE (m_rid, g_rid)
    );
)";

static constexpr const char* SQL_INSERT = "INSERT INTO GROUPMAPPING (m_rid, g_rid) VALUES (?, ?);";
static constexpr const char* SQL_CHECK_OVER_GROUP_LIMIT = R"(
    SELECT 1
    FROM GROUPMAPPING
    WHERE g_rid IN (%s)
    GROUP BY m_rid
    HAVING COUNT(g_rid) > ?
    LIMIT 1;
)";
static constexpr const char* SQL_CHECK_OVER_EMAIL_LIMIT = R"(
    SELECT 1
    FROM GROUPMAPPING
    WHERE m_rid IN (%s)
    GROUP BY g_rid
    HAVING COUNT(m_rid) > ?
    LIMIT 1;
)";
static constexpr const char* SQL_REMOVE_BY_EMAIL_RID = R"(DELETE FROM GROUPMAPPING WHERE m_rid = ?;)";

namespace addrbook {
EmailGroupDao::EmailGroupDao()
  : AbstractDao(SQL_TABLE_NAME)
{
}

bool EmailGroupDao::Create()
{
    return AbstractDao::OnExecuteSql(SQL_CREATE_TABLE);
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
    return AbstractDao::OnExecuteSql(SQL_INSERT, stmt_paramss);
}

bool EmailGroupDao::HasMemberOverGroupLimit(const std::vector<uint32_t>& group_ids, uint32_t limit) const
{
    return CheckMemberLimit(group_ids, SQL_CHECK_OVER_GROUP_LIMIT, limit);
}

bool EmailGroupDao::HasMemberOverEMailLimit(const std::vector<uint32_t>& email_ids, uint32_t limit) const
{
    return CheckMemberLimit(email_ids, SQL_CHECK_OVER_EMAIL_LIMIT, limit);
}

bool EmailGroupDao::CheckMemberLimit(const std::vector<uint32_t>& ids, const std::string& content, uint32_t limit) const
{
    bool ret = false;
    std::string str_ids = AddrMgrUtilities::JoinIds(ids);
    std::string str_sql = AddrMgrUtilities::ReplaceFirst(content, str_ids);

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
    return AbstractDao::OnExecuteSql(SQL_REMOVE_BY_EMAIL_RID, {param});
}
}  // namespace addrbook