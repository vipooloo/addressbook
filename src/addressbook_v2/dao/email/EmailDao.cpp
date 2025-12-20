#include "AddrCenterLog.h"
#include "EmailDao.h"
#include "EmailEntity.h"
#include <algorithm>
#include <iostream>
#include <sqlite3.h>
#include <sstream>

static constexpr const char* SQL_TABLE_NAME = "email";
static constexpr const char* SQL_CREATE_TABLE = R"(
CREATE TABLE IF NOT EXISTS email (
    rid INTEGER PRIMARY KEY AUTOINCREMENT, 
    email_address TEXT, 
    email_name TEXT
    );
)";
static constexpr const char* SQL_COUNT = "SELECT COUNT(*) FROM email;";
static constexpr const char* SQL_INSERT = "INSERT INTO email (email_address, email_name) VALUES (?, ?);";
static constexpr const char* SQL_DELETE_BY_RID = "DELETE FROM email WHERE rid = ?";
static constexpr const char* SQL_DELETE_ALL = "DELETE FROM email";
static constexpr const char* SQL_IS_EXIST = "SELECT COUNT(rid) FROM email WHERE rid IN (?);";

EmailDao::EmailDao()
  : AbstractDao()
{
}

bool EmailDao::Init()
{
    return AbstractDao::OnExecuteSql(SQL_CREATE_TABLE);
}

size_t EmailDao::GetCount() const
{
    return AbstractDao::OnGetCount(SQL_COUNT);
}

bool EmailDao::Insert(const std::shared_ptr<AbstractEntity>& in_entity_sptr, const std::shared_ptr<AbstractEntity>& out_entity_sptr)
{
    bool ret = false;
    std::shared_ptr<EmailEntity> email_entity_sptr = std::static_pointer_cast<EmailEntity>(in_entity_sptr);
    if (email_entity_sptr)
    {
        SQLite::Statement stmt(AbstractDao::GetDb(), SQL_INSERT);
        stmt.bind(1, email_entity_sptr->GetEmailAddress());
        stmt.bind(2, email_entity_sptr->GetEmailName());
        int32_t code = stmt.tryExecuteStep();
        if (SQLITE_DONE == code)
        {
            std::shared_ptr<EmailEntity> new_entity_sptr = std::static_pointer_cast<EmailEntity>(out_entity_sptr);
            if (new_entity_sptr)
            {
                new_entity_sptr->SetEmailAddress(email_entity_sptr->GetEmailAddress());
                new_entity_sptr->SetEmailName(email_entity_sptr->GetEmailName());
                int32_t rid = static_cast<int32_t>(AbstractDao::GetDb().getLastInsertRowid());
                new_entity_sptr->SetRid(rid);
                ret = true;
            }
        }
        else
        {
            AB_LOG_E("Insert email failed, code: %d", code);
        }
    }
    return ret;
}

bool EmailDao::IsExist(const std::vector<uint32_t>& rids)
{
    bool ret = false;
    if (!rids.empty())
    {
        // 去掉重复的
        std::vector<uint32_t> unique_rids = rids;
        std::sort(unique_rids.begin(), unique_rids.end());
        std::vector<uint32_t>::iterator last = std::unique(unique_rids.begin(), unique_rids.end());
        unique_rids.erase(last, unique_rids.end());

        SQLite::Statement stmt(AbstractDao::GetDb(), SQL_IS_EXIST);
        stmt.bind(1, AbstractDao::JoinIds(unique_rids));

        int32_t code = stmt.tryExecuteStep();
        if (SQLITE_ROW == code)
        {
            ret = stmt.getColumn(0).getInt() == 1;
        }
        else
        {
            AB_LOG_E("IsExist failed, code: %d", code);
        }
    }
    return ret;
}

bool EmailDao::Remove(const std::vector<uint32_t>& rids)
{
    bool ret = true;
    SQLite::Statement stmt(AbstractDao::GetDb(), SQL_DELETE_BY_RID);
    for (uint32_t rid : rids)
    {
        stmt.bind(1, rid);
        int32_t code = stmt.tryExecuteStep();
        if (SQLITE_DONE != code)
        {
            AB_LOG_E("Remove email failed, code: %d", code);
            ret = false;
            break;
        }
        stmt.reset();
        stmt.clearBindings();
    }
    return ret;
}

bool EmailDao::RemoveAll()
{
    return AbstractDao::OnExecuteSql(SQL_DELETE_ALL);
}