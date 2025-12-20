#include "AddrCenterLog.h"
#include "MailGroupDao.h"
#include "MailGroupRelation.h"
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

static constexpr const char* SQL_COUNT = "SELECT COUNT(*) FROM GROUPMAPPING;";
static constexpr const char* SQL_INSERT = "INSERT INTO GROUPMAPPING (m_rid, g_rid) VALUES (?, ?);";

MailGroupDao::MailGroupDao()
  : AbstractDao()
{
}

bool MailGroupDao::Init()
{
    return AbstractDao::OnExecuteSql(SQL_CREATE_TABLE);
}

size_t MailGroupDao::GetCount() const
{
    return AbstractDao::OnGetCount(SQL_COUNT);
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