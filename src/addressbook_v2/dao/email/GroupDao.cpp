#include "AddrCenterLog.h"
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

GroupDao::GroupDao()
  : AbstractDao(SQL_TABLE_NAME)
{
}

bool GroupDao::Create()
{
    return AbstractDao::OnExecuteSql(SQL_CREATE_TABLE);
}

bool GroupDao::Insert(const std::shared_ptr<AbstractEntity>& in_entity_sptr, const std::shared_ptr<AbstractEntity>& out_entity_sptr)
{
    bool ret = false;
    std::shared_ptr<GroupEntity> group_entity_sptr = std::static_pointer_cast<GroupEntity>(in_entity_sptr);
    if (group_entity_sptr)
    {
        SQLite::Statement stmt(AbstractDao::GetDb(), SQL_INSERT);
        stmt.bind(1, group_entity_sptr->GetGroupName());
        int32_t code = stmt.tryExecuteStep();
        if (SQLITE_DONE == code)
        {
            std::shared_ptr<GroupEntity> new_entity_sptr = std::static_pointer_cast<GroupEntity>(out_entity_sptr);
            if (new_entity_sptr)
            {
                new_entity_sptr->SetGroupName(group_entity_sptr->GetGroupName());
                int32_t rid = static_cast<int32_t>(AbstractDao::GetDb().getLastInsertRowid());
                new_entity_sptr->SetRid(rid);
                ret = true;
            }
        }
        else
        {
            AB_LOG_E("Insert group failed, code: %d", code);
        }
    }
    return ret;
}
