#include "EmailDao.h"
#include "EmailEntity.h"
#include <sqlite3.h>

static constexpr const char* SQL_TABLE_NAME = "email";
static constexpr const char* SQL_CREATE_TABLE = "CREATE TABLE IF NOT EXISTS email (rid INTEGER PRIMARY KEY AUTOINCREMENT, email_address TEXT, email_name TEXT);";
static constexpr const char* SQL_COUNT = "SELECT COUNT(*) FROM email;";
static constexpr const char* SQL_INSERT = "INSERT INTO email (email_address, email_name) VALUES (?, ?);";

EmailDao::EmailDao()
  : AbstractDao()
{
}

bool EmailDao::Init()
{
    return AbstractDao::OnCreteTable(SQL_CREATE_TABLE);
}

size_t EmailDao::GetCount() const
{
    return AbstractDao::OnGetCount(SQL_COUNT);
}

bool EmailDao::Insert(const AbstractEntity& entity, std::shared_ptr<AbstractEntity>& out_entity_sptr)
{
    const EmailEntity& email_entity = static_cast<const EmailEntity&>(entity);
    SQLite::Statement stmt(AbstractDao::GetDb(), SQL_INSERT);
    stmt.bind(1, email_entity.GetEmailAddress());
    stmt.bind(2, email_entity.GetEmailName());

    bool ret = false;
    if (SQLITE_DONE == stmt.tryExecuteStep())
    {
        std::shared_ptr<EmailEntity> new_entity = std::static_pointer_cast<EmailEntity>(out_entity_sptr);
        if (new_entity)
        {
            new_entity->SetEmailAddress(email_entity.GetEmailAddress());
            new_entity->SetEmailName(email_entity.GetEmailName());
            int32_t rid = static_cast<int32_t>(AbstractDao::GetDb().getLastInsertRowid());
            new_entity->SetRid(rid);
            ret = true;
        }
    }
    return ret;
}