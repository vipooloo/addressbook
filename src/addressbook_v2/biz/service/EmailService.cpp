#include "EmailRepository.h"
#include "EmailService.h"

ErrorCode EmailService::AddEmail(const EmailDto& dto)
{
    ErrorCode ret = ErrorCode::kSuccess;

    do
    {
        if (dto.GetName().empty() || dto.GetAddress().empty())
        {
            ret = ErrorCode::kInvalidParam;
            break;
        }
        if (!m_repo_sptr)
        {
            ret = ErrorCode::kDbError;
            break;
        }
        EmailEntity entity;
        entity.SetEmailName(dto.GetName());
        entity.SetEmailAddress(dto.GetAddress());
        ret = m_repo_sptr->AddEmail(entity);
    } while (false);

    return ret;
}