#include "AddrMgrLog.h"
#include "AddrMgrSqlDefs.h"
#include "GroupDao.h"
#include "GroupEntity.h"
#include <sqlite3.h>

namespace addrbook {
GroupDao::GroupDao()
  : AbstractDao(SQL_GROUP_TABLE_NAME)
{
}

bool GroupDao::Create()
{
    return AbstractDao::OnExecuteSql(SQL_GROUP_CREATE_TABLE);
}

std::pair<bool, GroupEntity> GroupDao::Insert(const GroupEntity& entity)
{
    std::pair<bool, GroupEntity> result = {false, entity};

    SQLite::Statement stmt(AbstractDao::GetDb(), SQL_GROUP_INSERT);
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
        AB_LOG_E("%s failed code:%d", SQL_GROUP_INSERT, code);
    }

    return result;
}

}  // namespace addrbook