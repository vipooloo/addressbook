#include "AddrCenterLog.h"
#include "GroupDao.h"
#include "GroupEntity.h"
#include <sqlite3.h>
#include <sstream>

static constexpr const char* SQL_TABLE_NAME = "mail_group";
static constexpr const char* SQL_CREATE_TABLE = R"(
CREATE TABLE IF NOT EXISTS mail_group (
    rid INTEGER PRIMARY KEY AUTOINCREMENT, 
    group_name TEXT
    );
)";
static constexpr const char* SQL_COUNT = "SELECT COUNT(*) FROM mail_group;";
static constexpr const char* SQL_INSERT = "INSERT INTO mail_group (group_name) VALUES (?);";

GroupDao::GroupDao()
  : AbstractDao()
{
}

bool GroupDao::Init()
{
    return AbstractDao::OnExecuteSql(SQL_CREATE_TABLE);
}

size_t GroupDao::GetCount() const
{
    return AbstractDao::OnGetCount(SQL_COUNT);
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

bool GroupDao::IsExist(const std::vector<uint32_t>& rids)
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
            AB_LOG_E("IsExist group failed, code: %d", code);
        }
    }
    return ret;
}