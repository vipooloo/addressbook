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

uint32_t GroupDao::GetGroupRid(const GroupEntity& entity)
{
    uint32_t rid = 0;

    SQLite::Statement stmt(AbstractDao::GetDb(), SQL_SELECT_GROUP_BY_NAME);
    stmt.bind(1, entity.GetGroupName());

    int32_t code = stmt.tryExecuteStep();
    if (SQLITE_ROW == code)
    {
        rid = stmt.getColumn(0).getUInt();
    }
    else if (SQLITE_DONE == code)
    {
        rid = 0;
    }
    else
    {
        AB_LOG_E("%s failed code:%d", SQL_SELECT_GROUP_BY_NAME, code);
    }

    return rid;
}

bool GroupDao::Update(const GroupEntity& entity)
{
    std::vector<StmtParam> stmt_params;
    stmt_params.emplace_back(entity.GetGroupName());
    stmt_params.emplace_back(entity.GetRid());

    return OnExecuteSql(SQL_GROUP_UPDATE, stmt_params);
}

}  // namespace addrbook