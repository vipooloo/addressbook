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

DaoErrCode EmailDao::Insert(const AbstractEntity& entity)
{
    const EmailEntity& email_entity = static_cast<const EmailEntity&>(entity);
    SQLite::Statement stmt(AbstractDao::GetDb(), SQL_INSERT);
    stmt.bind(1, email_entity.GetEmailAddress());
    stmt.bind(2, email_entity.GetEmailName());

    DaoErrCode ret = DaoErrCode::INTERNAL_ERROR;
    if (SQLITE_DONE == stmt.tryExecuteStep())
    {
        ret = DaoErrCode::SUCCESS;
    }
    return ret;
}