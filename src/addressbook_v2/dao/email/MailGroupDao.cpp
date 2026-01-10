#include "AddrMgrLog.h"
#include "AddrMgrUtilities.h"
#include "MailGroupDao.h"
#include "MailGroupRelation.h"
#include <algorithm>
#include <array>
#include <sqlite3.h>

static constexpr uint32_t SQL_BUFFER_SIZE = 512;
static constexpr const char* SQL_TABLE_NAME = "GROUPMAPPING";
static constexpr const char* SQL_CREATE_TABLE = R"(
CREATE TABLE IF NOT EXISTS GROUPMAPPING (
    id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    m_rid INTEGER,  
    g_rid INTEGER,  
    FOREIGN KEY (g_rid) REFERENCES MAILGROUP (rid) ON DELETE SET NULL ON UPDATE CASCADE,
    FOREIGN KEY (m_rid) REFERENCES MAIL      (rid) ON DELETE SET NULL ON UPDATE CASCADE,
    UNIQUE (m_rid, g_rid)
    );
)";

static constexpr const char* SQL_INSERT = "INSERT INTO GROUPMAPPING (m_rid, g_rid) VALUES (?, ?);";
static constexpr const char* SQL_QUERY_GRID_BY_RID = "SELECT g_rid FROM GROUPMAPPING WHERE rid = ?;";
static constexpr const char* SQL_QUERY_MAIL_BY_RID = "SELECT m_rid FROM GROUPMAPPING WHERE rid = ?;";
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
MailGroupDao::MailGroupDao()
  : AbstractDao(SQL_TABLE_NAME)
{
}

bool MailGroupDao::Create()
{
    return AbstractDao::OnExecuteSql(SQL_CREATE_TABLE);
}

bool MailGroupDao::InsertBatch(const std::vector<MailGroupRelation>& items)
{
    std::vector<std::vector<StmtParam>> stmt_paramss;
    std::transform(
        items.cbegin(),
        items.cend(),
        std::back_inserter(stmt_paramss),
        [](const MailGroupRelation& item) {
            std::vector<StmtParam> stmt_params;
            stmt_params.emplace_back(item.GetMailRid());
            stmt_params.emplace_back(item.GetGroupRid());
            return stmt_params;
        });
    return AbstractDao::OnExecuteSql(SQL_INSERT, stmt_paramss);
}

bool MailGroupDao::HasMemberOverGroupLimit(const std::vector<uint32_t>& group_ids, uint32_t limit) const
{
    return HasMemberOverLimit(group_ids, SQL_CHECK_OVER_GROUP_LIMIT, limit);
}

bool MailGroupDao::HasMemberOverEMailLimit(const std::vector<uint32_t>& group_ids, uint32_t limit) const
{
    return HasMemberOverLimit(group_ids, SQL_CHECK_OVER_EMAIL_LIMIT, limit);
}

bool MailGroupDao::HasMemberOverLimit(const std::vector<uint32_t>& ids, const std::string& content, uint32_t limit) const
{
    std::string str_ids = AddrMgrUtilities::JoinIds(ids);
    std::array<char, SQL_BUFFER_SIZE> sql_buffer = {0};
    snprintf(sql_buffer.data(), sql_buffer.size(), content.c_str(), str_ids.c_str());

    bool ret = true;
    SQLite::Statement stmt(AbstractDao::GetDb(), sql_buffer.data());
    stmt.bind(1, limit);
    int32_t code = stmt.tryExecuteStep();
    if (SQLITE_ROW == code)
    {
        AB_LOG_E("%s failed code:%d", str_ids.c_str(), code);
        ret = false;
    }
    return ret;
}

bool MailGroupDao::RemoveByEmailRid(uint32_t email_rid)
{
    StmtParam param(email_rid);
    return AbstractDao::OnExecuteSql(SQL_REMOVE_BY_EMAIL_RID, {param});
}
}  // namespace addrbook