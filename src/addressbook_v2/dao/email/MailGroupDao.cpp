#include "AddrCenterLog.h"
#include "MailGroupDao.h"
#include "MailGroupRelation.h"
#include <algorithm>
#include <sqlite3.h>

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

MailGroupDao::MailGroupDao()
  : AbstractDao(SQL_TABLE_NAME)
{
}

bool MailGroupDao::Init()
{
    return AbstractDao::OnExecuteSql(SQL_CREATE_TABLE);
}

size_t MailGroupDao::CountByCond(const MailGroupMappingQueryCond& cond) const
{
    //TBD
    return cond.InValid() ? GetCount() : 0;
}

bool MailGroupDao::Insert(const std::shared_ptr<AbstractEntity>& in_entity_sptr, const std::shared_ptr<AbstractEntity>& out_entity_sptr)
{
    (void)out_entity_sptr;
    bool ret = false;
    std::shared_ptr<MailGroupRelation> relation_entity_sptr = std::static_pointer_cast<MailGroupRelation>(in_entity_sptr);
    if (relation_entity_sptr)
    {
        SQLite::Statement stmt(AbstractDao::GetDb(), SQL_INSERT);
        stmt.bind(1, relation_entity_sptr->GetMailRid());
        stmt.bind(2, relation_entity_sptr->GetGroupRid());
        int32_t code = stmt.tryExecuteStep();
        if (SQLITE_DONE == code)
        {
            ret = true;
        }
        else
        {
            AB_LOG_E("Insert failed, code:%d", code);
        }
    }
    return ret;
}

bool MailGroupDao::InsertBatch(const std::vector<std::shared_ptr<AbstractEntity>>& items)
{
    bool ret = true;
    SQLite::Statement stmt(AbstractDao::GetDb(), SQL_INSERT);
    for (const std::shared_ptr<AbstractEntity>& item : items)
    {
        std::shared_ptr<MailGroupRelation> entity_sptr = std::static_pointer_cast<MailGroupRelation>(item);
        if (entity_sptr)
        {
            stmt.bind(1, entity_sptr->GetMailRid());
            stmt.bind(2, entity_sptr->GetGroupRid());
            int32_t code = stmt.tryExecuteStep();
            if (SQLITE_DONE != code)
            {
                AB_LOG_E("InsertBatch failed, ret_code:%d", code);
                ret = false;
                break;
            }
            stmt.reset();
            stmt.clearBindings();
        }
        else
        {
            ret = false;
            break;
        }
    }
    return ret;
}

std::map<uint32_t, uint32_t> MailGroupDao::GetGroupEmailCounts(const std::vector<uint32_t>& rids, bool is_mail) const
{
    std::map<uint32_t, uint32_t> ret_count;
    // 默认值
    std::for_each(rids.cbegin(), rids.cend(), [&ret_count](uint32_t group_id) {
        ret_count[group_id] = 0;
    });
    std::string str_ids = AbstractDao::JoinIds(rids);
    std::string sql = "SELECT g_rid, COUNT(*) FROM " + std::string(SQL_TABLE_NAME) + " WHERE g_rid IN (" + str_ids + ") GROUP BY g_rid;";
    if (is_mail)
    {
        sql = "SELECT m_rid, COUNT(*) FROM " + std::string(SQL_TABLE_NAME) + " WHERE m_rid IN (" + str_ids + ") GROUP BY m_rid;";
    }
    SQLite::Statement stmt(GetDb(), sql);
    int32_t code = stmt.tryExecuteStep();
    while (SQLITE_ROW == code)
    {
        uint32_t g_rid = static_cast<uint32_t>(stmt.getColumn(0).getUInt());
        uint32_t count = static_cast<uint32_t>(stmt.getColumn(1).getUInt());
        ret_count[g_rid] = count;
        code = stmt.tryExecuteStep();
    }
    return ret_count;
}

std::vector<uint32_t> MailGroupDao::GetEmailGroupsByEmailId(uint32_t email_id) const
{
    std::string sql = "SELECT g_rid FROM GROUPMAPPING WHERE rid = ?;";
    SQLite::Statement stmt(AbstractDao::GetDb(), sql);
    stmt.bind(1, email_id);
    std::vector<uint32_t> ret_group_ids;
    int32_t code = stmt.tryExecuteStep();
    while (SQLITE_ROW == code)
    {
        ret_group_ids.push_back(static_cast<uint32_t>(stmt.getColumn(0).getUInt()));
        code = stmt.tryExecuteStep();
    }
    return ret_group_ids;
}
std::vector<uint32_t> MailGroupDao::GetEmailIdsByGroupId(uint32_t group_id) const
{
    std::string sql = "SELECT m_rid FROM GROUPMAPPING WHERE rid = ?;";
    SQLite::Statement stmt(AbstractDao::GetDb(), sql);
    stmt.bind(1, group_id);
    std::vector<uint32_t> ret_email_ids;
    int32_t code = stmt.tryExecuteStep();
    while (SQLITE_ROW == code)
    {
        ret_email_ids.push_back(static_cast<uint32_t>(stmt.getColumn(0).getUInt()));
        code = stmt.tryExecuteStep();
    }
    return ret_email_ids;
}