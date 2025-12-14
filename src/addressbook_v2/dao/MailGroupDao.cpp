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
    FOREIGN KEY (m_rid) REFERENCES MAIL (rid) ON DELETE SET NULL ON UPDATE CASCADE,
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
    return AbstractDao::OnCreteTable(SQL_CREATE_TABLE);
}

size_t MailGroupDao::GetCount() const
{
    return AbstractDao::OnGetCount(SQL_COUNT);
}

size_t MailGroupDao::CountByCond(const MailGroupMappingQueryCond& cond) const
{
    size_t ret_count = 0;
    if (cond.InValid())
    {
        ret_count = GetCount();
    }
    else
    {
        // TODO
    }
    return ret_count;
}

bool MailGroupDao::Insert(const AbstractEntity& entity, std::shared_ptr<AbstractEntity>& out_entity_sptr)
{
    (void)out_entity_sptr;

    const MailGroupRelation& relation_entity = static_cast<const MailGroupRelation&>(entity);
    SQLite::Statement stmt(AbstractDao::GetDb(), SQL_INSERT);
    stmt.bind(1, relation_entity.GetMailRid());
    stmt.bind(2, relation_entity.GetGroupRid());
    return SQLITE_DONE == stmt.tryExecuteStep();
}