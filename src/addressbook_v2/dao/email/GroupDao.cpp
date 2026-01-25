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

PageResult GroupDao::FindByPage(const PageQueryParam& query_param)
{
    CustomWhere conditions(SQL_GROUP_SELECT_WITH_EMAIL_BY_PAGE, SQL_GROUP_WHERE_SEARCH_KEYWORD);
    conditions.AddWhereArg("%" + query_param.GetKeyword() + "%");

    return AbstractDao::DoFindByPage(query_param, conditions);
}

PageResult GroupDao::FindAll(const PageQueryParam& query_param)
{
    CustomWhere conditions(SQL_GROUP_SELECT_WITH_GROUPS_BY_PAGE, SQL_EMAIL_WHERE_NO_FILTER);

    return AbstractDao::DoFindByPage(query_param, conditions);
}

std::shared_ptr<AbstractEntity> GroupDao::OnCreateEntity(const SQLite::Statement& stmt)
{
    uint32_t rid = static_cast<uint32_t>(stmt.getColumn(0).getUInt());  // rid
    std::string group_name = stmt.getColumn(1).getString();             // group_name
    std::string mail_rids = stmt.getColumn(2).getString();              // mail_rids
    std::string email_names = stmt.getColumn(3).getString();            // email_names
    std::string email_addresses = stmt.getColumn(4).getString();        // email_addresses

    return std::make_shared<GroupEntity>(rid, group_name, mail_rids, email_names, email_addresses);
}

}  // namespace addrbook