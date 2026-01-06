#include "AddrCenterLog.h"
#include "EmailDao.h"
#include "EmailEntity.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <sqlite3.h>

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
        ret = true;
    }
    return ret;
}

std::shared_ptr<AbstractEntity> EmailDao::OnCreateEntity(const SQLite::Statement& stmt)
{
    uint32_t rid = static_cast<uint32_t>(stmt.getColumn(0).getUInt());  // rid
    std::string email_address = stmt.getColumn(1).getString();          // email_address
    std::string email_name = stmt.getColumn(2).getString();             // email_name
    std::string group_rids = stmt.getColumn(3).getString();             // group_rids
    std::string group_names = stmt.getColumn(4).getString();            // group_names
    return std::make_shared<EmailEntity>(rid, email_address, email_name, group_rids, group_names);
}

PageResult EmailDao::FindByPage(const std::string& keyword, uint32_t page_num, uint32_t page_size)
{
    static constexpr const char* sql = R"(
            SELECT 
                m.rid, 
                m.email_address, 
                m.email_name, 
                GROUP_CONCAT(g.rid, ',') as group_rids, 
                GROUP_CONCAT(g.group_name, '|##|') as group_names 
            FROM %s m 
            LEFT JOIN GROUPMAPPING r ON m.rid = r.m_rid 
            LEFT JOIN mail_group g ON r.g_rid = g.rid 
            WHERE %s 
            GROUP BY m.rid 
            ORDER BY m.rid %s LIMIT ? OFFSET ?;
)";
    static constexpr const char* where_sql = "email_address LIKE ? OR email_name LIKE ?";
    CustomWhere conditions(sql, where_sql);
    conditions.AddWhereArg("%" + keyword + "%");
    conditions.AddWhereArg("%" + keyword + "%");
    QueryParams params(page_num, page_size, conditions);
    return AbstractDao::DoFindByPage(params);
}

PageResult EmailDao::FindAll(uint32_t page_num, uint32_t page_size)
{
    static constexpr const char* sql = R"(
            SELECT 
                m.rid, 
                m.email_address, 
                m.email_name, 
                GROUP_CONCAT(g.rid, ',') as group_rids, 
                GROUP_CONCAT(g.group_name, '|##|') as group_names 
            FROM %s m 
            LEFT JOIN GROUPMAPPING r ON m.rid = r.m_rid 
            LEFT JOIN mail_group g ON r.g_rid = g.rid 
            WHERE %s 
            GROUP BY m.rid 
            ORDER BY m.rid %s LIMIT ? OFFSET ?;
)";
    static constexpr const char* where_sql = "1 = 1";
    CustomWhere conditions(sql, where_sql);
    QueryParams params(page_num, page_size, conditions);
    return AbstractDao::DoFindByPage(params);
}