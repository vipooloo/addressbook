#include "EmailDao.h"
#include <sqlite3.h>

static constexpr const char* SQL_TABLE_NAME = "email";
static constexpr const char* SQL_CREATE_TABLE = "CREATE TABLE IF NOT EXISTS email (rid INTEGER PRIMARY KEY AUTOINCREMENT, email_address TEXT, email_name TEXT);";
static constexpr const char* SQL_INSERT = "INSERT INTO email (email_address, email_name) VALUES (?, ?);";

EmailDao::EmailDao(const std::shared_ptr<SQLiteConn>& db_sptr)
  : AbstractDao(SQL_TABLE_NAME, db_sptr)
{
}

bool EmailDao::Init()
{
    SQLite::Statement stmt(GetDb(), SQL_CREATE_TABLE);
    return (SQLITE_DONE == stmt.tryExecuteStep());
}

DaoErrCode EmailDao::Insert(const AbstractEntity& entity)
{
    const EmailEntity& email_entity = static_cast<const EmailEntity&>(entity);
    SQLite::Statement stmt(GetDb(), SQL_INSERT);
    stmt.bind(1, email_entity.GetEmailAddress());
    stmt.bind(2, email_entity.GetEmailName());

    DaoErrCode ret = DaoErrCode::INTERNAL_ERROR;
    if (SQLITE_DONE == stmt.tryExecuteStep())
    {
        ret = DaoErrCode::SUCCESS;
    }
    return ret;
}