#include "AddrCenterLog.h"
#include "EmailDao.h"
#include "EmailEntity.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <sqlite3.h>

static constexpr uint32_t SQL_BUFFER_SIZE = 512;
static constexpr const char* SQL_TABLE_NAME = "email";
static constexpr const char* SQL_CREATE_TABLE = R"(
CREATE TABLE IF NOT EXISTS email (
    rid INTEGER PRIMARY KEY AUTOINCREMENT, 
    email_address TEXT, 
    email_name TEXT
    );
)";
static constexpr const char* SQL_INSERT = "INSERT INTO email (email_address, email_name) VALUES (?, ?);";
static constexpr const char* SQL_UPDATE = "UPDATE email SET email_address = ?, email_name = ? WHERE rid = ?;";

EmailDao::EmailDao()
  : AbstractDao(SQL_TABLE_NAME)
{
}

bool EmailDao::Create()
{
    return AbstractDao::OnExecuteSql(SQL_CREATE_TABLE);
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

bool EmailDao::Update(const std::shared_ptr<AbstractEntity>& entity_sptr)
{
    bool ret = false;
    std::shared_ptr<EmailEntity> email_entity_sptr = std::static_pointer_cast<EmailEntity>(entity_sptr);
    if (email_entity_sptr)
    {
        std::vector<StmtParam> stmt_params;
        stmt_params.emplace_back(email_entity_sptr->GetEmailAddress());
        stmt_params.emplace_back(email_entity_sptr->GetEmailName());
        stmt_params.emplace_back(email_entity_sptr->GetRid());
        ret = OnExecuteSql(SQL_UPDATE, stmt_params);
    }
    return ret;
}

std::shared_ptr<AbstractEntity> EmailDao::OnCreateEntity(const SQLite::Statement& stmt)
{
    uint32_t rid = static_cast<uint32_t>(stmt.getColumn(0).getUInt());  // rid
    std::string email_address = stmt.getColumn(1).getString();          // email_address
    std::string email_name = stmt.getColumn(2).getString();             // email_name
    return std::make_shared<EmailEntity>(rid, email_address, email_name);
}