#include "AddrMgrLog.h"
#include "GroupDao.h"
#include "GroupEntity.h"
#include <array>
#include <sqlite3.h>

static constexpr const char* SQL_TABLE_NAME = "mail_group";
static constexpr const char* SQL_CREATE_TABLE = R"(
CREATE TABLE IF NOT EXISTS mail_group (
    rid INTEGER PRIMARY KEY AUTOINCREMENT, 
    group_name TEXT
    );
)";
static constexpr const char* SQL_INSERT = "INSERT INTO mail_group (group_name) VALUES (?);";

namespace addrbook {
GroupDao::GroupDao()
  : AbstractDao(SQL_TABLE_NAME)
{
}

bool GroupDao::Create()
{
    return AbstractDao::OnExecuteSql(SQL_CREATE_TABLE);
}

std::pair<bool, GroupEntity> GroupDao::Insert(const GroupEntity& entity)
{
    std::pair<bool, GroupEntity> result = {false, entity};

    SQLite::Statement stmt(AbstractDao::GetDb(), SQL_INSERT);
    stmt.bind(1, entity.GetGroupName());
    int32_t code = stmt.tryExecuteStep();
    if (SQLITE_DONE == code)
    {
        int32_t rid = static_cast<int32_t>(AbstractDao::GetDb().getLastInsertRowid());
        result.first = true;
        result.second.SetRid(rid);
    }
    else
    {
        AB_LOG_E("Insert group failed, code: %d", code);
    }

    return result;
}

}  // namespace addrbook