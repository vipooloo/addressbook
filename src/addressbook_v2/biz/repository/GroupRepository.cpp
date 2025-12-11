#include "AddrCenter.h"
#include "GroupDao.h"
#include "GroupRepository.h"

GroupRepository::GroupRepository()
  : m_group_dao_sptr{std::make_shared<GroupDao>()}
{
    if (m_group_dao_sptr)
    {
        m_group_dao_sptr->Init();
    }
}

ErrorCode GroupRepository::AddGroup(const GroupEntity& entity)
{
    ErrorCode result = ErrorCode::kDbError;
    if (m_group_dao_sptr)
    {
        TransactionGuard db_trans_guard;
        DaoErrCode dao_result = m_group_dao_sptr->Insert(entity);
        switch (dao_result)
        {
            case DaoErrCode::SUCCESS:
                result = ErrorCode::kSuccess;
                break;
            case DaoErrCode::INVALID_PARAM:
                result = ErrorCode::kInvalidParam;
                break;
            default:
                result = ErrorCode::kDbError;
                break;
        }
    }
    return result;
}

size_t GroupRepository::GetGroupCount() const
{
    size_t count = 0;
    if (m_group_dao_sptr)
    {
        count = m_group_dao_sptr->GetCount();
    }
    return count;
}