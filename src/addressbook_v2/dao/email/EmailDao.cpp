#include "AddrCenterLog.h"
#include "EmailDao.h"
#include "EmailEntity.h"
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
        std::ostringstream sql;
        sql << "SELECT COUNT(DISTINCT rid) = " << rids.size()
            << " FROM " << SQL_TABLE_NAME << " WHERE rid IN (";

        for (size_t i = 0; i < rids.size(); ++i)
        {
            if (i > 0)
            {
                sql << ",";
            }
            sql << rids[i];
        }
        sql << ");";

        SQLite::Statement stmt(AbstractDao::GetDb(), sql.str());
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