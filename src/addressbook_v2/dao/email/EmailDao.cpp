#include "AddrMgrLog.h"
#include "AddrMgrSqlDefs.h"
#include "EmailDao.h"
#include <algorithm>
#include <iostream>
#include <sqlite3.h>

namespace addrbook {
EmailDao::EmailDao()
  : AbstractDao(SQL_EMAIL_TABLE_NAME)
{
}

bool EmailDao::Create()
{
    return AbstractDao::OnExecuteSql(SQL_EMAIL_CREATE_TABLE);
}

std::pair<bool, EmailEntity> EmailDao::Insert(const EmailEntity& entity)
{
    std::pair<bool, EmailEntity> result = {false, entity};

    SQLite::Statement stmt(AbstractDao::GetDb(), SQL_EMAIL_INSERT);
    stmt.bind(1, entity.GetEmailAddress());
    stmt.bind(2, entity.GetEmailName());

    int32_t code = stmt.tryExecuteStep();
    if (SQLITE_DONE == code)
    {
        int32_t rid = static_cast<int32_t>(AbstractDao::GetDb().getLastInsertRowid());
        result.first = true;
        result.second.SetRid(rid);
    }
    else
    {
        AB_LOG_E("%s failed code:%d", SQL_EMAIL_INSERT, code);
    }

    return result;
}

uint32_t EmailDao::GetEmailRid(const EmailEntity& entity)
{
    uint32_t rid = 0;

    SQLite::Statement stmt(AbstractDao::GetDb(), SQL_SELECT_EMAIL_BY_ADDRESS_AND_NAME);
    stmt.bind(1, entity.GetEmailAddress());
    stmt.bind(2, entity.GetEmailName());

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
        AB_LOG_E("%s failed code:%d", SQL_SELECT_EMAIL_BY_ADDRESS_AND_NAME, code);
    }

    return rid;
}

bool EmailDao::Update(const EmailEntity& entity)
{
    std::vector<StmtParam> stmt_params;
    stmt_params.emplace_back(entity.GetEmailAddress());
    stmt_params.emplace_back(entity.GetEmailName());
    stmt_params.emplace_back(entity.GetRid());

    return OnExecuteSql(SQL_EMAIL_UPDATE, stmt_params);
}

std::shared_ptr<AbstractEntity> EmailDao::OnCreateEntity(const SQLite::Statement& stmt)
{
    uint32_t rid = static_cast<uint32_t>(stmt.getColumn(0).getUInt());  // rid
    std::string email_name = stmt.getColumn(1).getString();             // email_address
    std::string email_address = stmt.getColumn(2).getString();          // email_name
    std::string group_rids = stmt.getColumn(3).getString();             // group_rids
    std::string group_names = stmt.getColumn(4).getString();            // group_names

    return std::make_shared<EmailEntity>(rid, email_address, email_name, group_rids, group_names);
}

PageResult EmailDao::FindByPage(const PageQueryParam& query_param)
{
    CustomWhere conditions(SQL_EMAIL_SELECT_WITH_GROUPS_BY_PAGE, SQL_EMAIL_WHERE_SEARCH_KEYWORD);
    conditions.AddWhereArg("%" + query_param.GetKeyword() + "%");
    conditions.AddWhereArg("%" + query_param.GetKeyword() + "%");

    return AbstractDao::DoFindByPage(query_param, conditions);
}

PageResult EmailDao::FindAll(const PageQueryParam& query_param)
{
    CustomWhere conditions(SQL_EMAIL_SELECT_WITH_GROUPS_BY_PAGE, SQL_EMAIL_WHERE_NO_FILTER);

    return AbstractDao::DoFindByPage(query_param, conditions);
}
}  // namespace addrbook