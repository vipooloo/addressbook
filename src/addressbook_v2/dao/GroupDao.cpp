#include "GroupDao.h"
#include "GroupEntity.h"
#include <sqlite3.h>

static constexpr const char* SQL_TABLE_NAME = "mail_group";
static constexpr const char* SQL_CREATE_TABLE = "CREATE TABLE IF NOT EXISTS mail_group (rid INTEGER PRIMARY KEY AUTOINCREMENT, group_name TEXT);";
static constexpr const char* SQL_COUNT = "SELECT COUNT(*) FROM mail_group;";
static constexpr const char* SQL_INSERT = "INSERT INTO mail_group (group_name) VALUES (?);";

GroupDao::GroupDao()
  : AbstractDao()
{
}

bool GroupDao::Init()
{
    return AbstractDao::OnCreteTable(SQL_CREATE_TABLE);
}

size_t GroupDao::GetCount() const
{
    return AbstractDao::OnGetCount(SQL_COUNT);
}

DaoErrCode GroupDao::Insert(const AbstractEntity& entity)
{
    const GroupEntity& group_entity = static_cast<const GroupEntity&>(entity);
    SQLite::Statement stmt(AbstractDao::GetDb(), SQL_INSERT);
    stmt.bind(1, group_entity.GetGroupName());

    DaoErrCode ret = DaoErrCode::INTERNAL_ERROR;
    if (SQLITE_DONE == stmt.tryExecuteStep())
    {
        ret = DaoErrCode::SUCCESS;
    }
    return ret;
}