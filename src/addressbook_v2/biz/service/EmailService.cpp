#include "EmailEntity.h"
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
        EmailEntity entity;
        entity.SetEmailName(dto.GetName());
        entity.SetEmailAddress(dto.GetAddress());
        ret = m_repo.AddEmail(entity);
    } while (false);
    return ret;
}