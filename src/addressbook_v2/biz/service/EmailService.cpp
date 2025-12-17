#include "AddrCenterLog.h"
#include "AddressBookConfigDefs.h"
#include "EmailEntity.h"
#include "EmailRepository.h"
#include "EmailService.h"
#include "TransactionGuard.h"
#include <algorithm>

EmailService::EmailService()
  : m_mail_rep_sptr{std::make_shared<EmailRepository>()}
{
}

std::pair<ErrorCode, EmailDto> EmailService::AddEmail(const EmailDto& dto)
{
    std::pair<ErrorCode, EmailDto> result = std::make_pair(ErrorCode::kSuccess, dto);
    ErrorCode& ret = result.first;
    do
    {
        if (!m_mail_rep_sptr)
        {
            AB_LOG_E("Email repository is null");
            ret = ErrorCode::kNotable;
            break;
        }
        // 输入合法验证
        if (dto.GetName().empty() || dto.GetAddress().empty())
        {
            AB_LOG_E("Invalid email name or address");
            ret = ErrorCode::kInvalidParam;
            break;
        }
        TransactionGuard trans_guard(true);
        // 检查邮件是否超过最大数量限制
        if (m_mail_rep_sptr->GetEmailCount() >= kMaxMailCount)
        {
            AB_LOG_E("Exceed max email count");
            ret = ErrorCode::kExceedMaxCount;
            break;
        }
        // 验证组是否存在
        std::vector<uint32_t> group_rids = dto.GetGroupRids();
        bool group_exist = m_mail_rep_sptr->IsGroupExist(group_rids);
        if (!group_exist)
        {
            AB_LOG_E("Some groups do not exist");
            ret = ErrorCode::kInvalidParam;
            break;
        }
        // 检查组中是否还能关联邮件
        if (!m_mail_rep_sptr->CanAddEmail(group_rids, kMaxGroupsPerEmail))
        {
            AB_LOG_E("Some groups exceed max email count");
            ret = ErrorCode::kExceedMaxCount;
            break;
        }
        // 添加邮件
        std::shared_ptr<EmailEntity> mail_entity_sptr = std::make_shared<EmailEntity>(dto.GetName(), dto.GetAddress());
        std::shared_ptr<AbstractEntity> out_entity_sptr = std::make_shared<EmailEntity>();
        bool add_mail_res = m_mail_rep_sptr->AddEmail(mail_entity_sptr, out_entity_sptr);
        if (!out_entity_sptr || !add_mail_res)
        {
            AB_LOG_E("Failed to add email to database");
            ret = ErrorCode::kDbError;
            break;
        }
        // 添加邮件到组的映射关系
        std::vector<std::shared_ptr<AbstractEntity>> relations;
        relations.reserve(group_rids.size());
        std::transform(
            group_rids.cbegin(),
            group_rids.cend(),
            std::back_inserter(relations),
            [out_entity_sptr](uint32_t group_rid) {
                return std::make_shared<MailGroupRelation>(out_entity_sptr->GetRid(), group_rid);
            });
        if (!m_mail_rep_sptr->AddEmailToGroupRelation(relations))
        {
            AB_LOG_E("Failed to add email-group relation to database");
            ret = ErrorCode::kDbError;
            break;
        }
        if (ErrorCode::kSuccess == ret)
        {
            result.second.SetRid(out_entity_sptr->GetRid());
            trans_guard.SetError(false);
        }
    } while (false);
    return result;
}

std::pair<ErrorCode, GroupDto> EmailService::AddGroup(const GroupDto& dto)
{
    std::pair<ErrorCode, GroupDto> result = std::make_pair(ErrorCode::kSuccess, dto);
    ErrorCode& ret = result.first;
    do
    {
        if (!m_mail_rep_sptr)
        {
            AB_LOG_E("Email repository is null");
            ret = ErrorCode::kNotable;
            break;
        }
        // 输入合法验证
        if (dto.GetGroupName().empty())
        {
            AB_LOG_E("Invalid group name");
            ret = ErrorCode::kInvalidParam;
            break;
        }
        TransactionGuard trans_guard(true);
        // 检查邮件组是否超过最大数量限制
        if (m_mail_rep_sptr->GetGroupCount() >= kMaxGroupCount)
        {
            AB_LOG_E("Exceed max group count");
            ret = ErrorCode::kExceedMaxCount;
            break;
        }
        // 邮件是否存在
        std::vector<uint32_t> mail_rids = dto.GetMailRids();
        bool group_exist = m_mail_rep_sptr->IsMailExist(mail_rids);
        if (!group_exist)
        {
            AB_LOG_E("Some mails do not exist");
            ret = ErrorCode::kInvalidParam;
            break;
        }
        // 检查邮件是否还能关联邮件组
        if (!m_mail_rep_sptr->CanAddGroup(mail_rids, kMaxEmailsPerGroup))
        {
            AB_LOG_E("Some mails exceed max email count");
            ret = ErrorCode::kExceedMaxCount;
            break;
        }
        // 添加邮件组
        std::shared_ptr<GroupEntity> group_entity_sptr = std::make_shared<GroupEntity>(dto.GetGroupName());
        std::shared_ptr<AbstractEntity> out_entity_sptr = std::make_shared<GroupEntity>();
        bool add_mail_res = m_mail_rep_sptr->AddGroup(group_entity_sptr, out_entity_sptr);
        if (!out_entity_sptr || !add_mail_res)
        {
            AB_LOG_E("Failed to add email to database");
            ret = ErrorCode::kDbError;
            break;
        }
        // 添加邮件到组的映射关系
        std::vector<std::shared_ptr<AbstractEntity>> relations;
        relations.reserve(mail_rids.size());
        std::transform(
            mail_rids.cbegin(),
            mail_rids.cend(),
            std::back_inserter(relations),
            [out_entity_sptr](uint32_t mail_rid) {
                return std::make_shared<MailGroupRelation>(mail_rid, out_entity_sptr->GetRid());
            });
        if (!m_mail_rep_sptr->AddEmailToGroupRelation(relations))
        {
            AB_LOG_E("Failed to add email-group relation to database");
            ret = ErrorCode::kDbError;
            break;
        }
        if (ErrorCode::kSuccess == ret)
        {
            result.second.SetRid(out_entity_sptr->GetRid());
            trans_guard.SetError(false);
        }
    } while (false);
    return result;
}