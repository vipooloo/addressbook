#include "AddrMgrLog.h"
#include "AddrMgrSqlDefs.h"
#include "GroupDao.h"
#include "GroupEntity.h"
#include <sqlite3.h>

namespace addrbook {
GroupDao::GroupDao()
  : AbstractDao(GROUP_SQL_TABLE_NAME)
{
}

bool GroupDao::Create()
{
    return AbstractDao::OnExecuteSql(GROUP_SQL_CREATE_TABLE);
}

std::pair<bool, GroupEntity> GroupDao::Insert(const GroupEntity& entity)
{
    std::pair<bool, GroupEntity> result = {false, entity};

    SQLite::Statement stmt(AbstractDao::GetDb(), GROUP_SQL_INSERT);
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
        AB_LOG_E("%s failed code:%d", GROUP_SQL_INSERT, code);
    }

    return result;
}

}  // namespace addrbook