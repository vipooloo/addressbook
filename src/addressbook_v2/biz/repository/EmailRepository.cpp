#include "EmailRepository.h"

EmailRepository::EmailRepository()
  : m_mail_dao_sptr{nullptr}
{
}

ErrorCode EmailRepository::AddEmail(const EmailEntity& entity)
{
    ErrorCode result = ErrorCode::kDbError;
    if (m_mail_dao_sptr)
    {
        DaoErrCode dao_result = m_mail_dao_sptr->Insert(entity);
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

uint32_t EmailRepository::GetEmailCount() const
{
    uint32_t count = 0;
    if (m_mail_dao_sptr)
    {
        count = m_mail_dao_sptr->GetCount();
    }
    return count;
}