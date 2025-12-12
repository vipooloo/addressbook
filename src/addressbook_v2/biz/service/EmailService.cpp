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
        std::vector<uint32_t> group_rids = dto.GetGroupRids();
        bool group_exist = m_group_repo.IsGroupExist(group_rids);
        if (!group_exist)
        {
            ret = ErrorCode::kNotFound;
            break;
        }
        if (m_mail_rep.GetEmailCount() >= 100)
        {
            ret = ErrorCode::kExceedMaxCount;
            break;
        }
        EmailEntity entity;
        entity.SetEmailName(dto.GetName());
        entity.SetEmailAddress(dto.GetAddress());
        ret = m_mail_rep.AddEmail(entity);
    } while (false);
    return ret;
}