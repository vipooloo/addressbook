#include "AddrCenterLog.h"
#include "AddressBookConfigDefs.h"
#include "AddressCenterUtilities.h"
#include "EmailEntity.h"
#include "EmailRepository.h"
#include "EmailService.h"
#include "PageResult.h"
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
        std::vector<uint32_t> group_rids = dto.GetGroupRids();
        // 检查其中是否还有rid=0
        if (std::any_of(group_rids.cbegin(), group_rids.cend(), [](uint32_t rid) {
                return rid == 0;
            }))
        {
            AB_LOG_E("Invalid group rid");
            ret = ErrorCode::kInvalidParam;
            break;
        }
        // 检查邮件组数量是否超了规格上限
        if (group_rids.size() > kMaxGroupsPerEmail)
        {
            AB_LOG_E("Exceed max group count");
            ret = ErrorCode::kExceedMaxCount;
            break;
        }
        TransactionGuard trans_guard;
        // 检查邮件是否超过最大数量限制
        if (m_mail_rep_sptr->GetEmailCount() >= kMaxMailCount)
        {
            AB_LOG_E("Exceed max email count");
            ret = ErrorCode::kExceedMaxCount;
            break;
        }
        // 检查邮件组是否存在
        bool group_exist = m_mail_rep_sptr->IsGroupExist(group_rids);
        if (!group_exist)
        {
            AB_LOG_E("Some groups do not exist");
            ret = ErrorCode::kInvalidParam;
            break;
        }
        // 检查邮件组中是否还能关联邮件
        if (!m_mail_rep_sptr->CanAddEmail(group_rids, kMaxGroupsPerEmail))
        {
            AB_LOG_E("Some groups exceed max email count");
            ret = ErrorCode::kExceedMaxCount;
            break;
        }
        // 添加邮件
        std::shared_ptr<EmailEntity> mail_entity_sptr = std::make_shared<EmailEntity>(dto.GetName(), dto.GetAddress());
        uint32_t rid = m_mail_rep_sptr->AddEmail(mail_entity_sptr, group_rids);
        if (0 == rid)
        {
            AB_LOG_E("Failed to add email to database");
            ret = ErrorCode::kDbError;
            break;
        }
        result.second.SetRid(rid);
        trans_guard.Commit();
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
        std::vector<uint32_t> mail_rids = dto.GetMailRids();
        // 检查其中是否还有rid=0
        if (std::any_of(mail_rids.cbegin(), mail_rids.cend(), [](uint32_t rid) {
                return rid == 0;
            }))
        {
            AB_LOG_E("Invalid mail rid");
            ret = ErrorCode::kInvalidParam;
            break;
        }
        // 检查邮件数量是否超了规格上限
        if (mail_rids.size() > kMaxEmailsPerGroup)
        {
            AB_LOG_E("Exceed max email count");
            ret = ErrorCode::kExceedMaxCount;
            break;
        }
        TransactionGuard trans_guard;
        // 检查邮件组是否超过最大数量限制
        if (m_mail_rep_sptr->GetGroupCount() >= kMaxGroupCount)
        {
            AB_LOG_E("Exceed max group count");
            ret = ErrorCode::kExceedMaxCount;
            break;
        }
        // 检查邮件是否存在
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
        uint32_t group_rid = m_mail_rep_sptr->AddGroup(group_entity_sptr, mail_rids);
        if (0 == group_rid)
        {
            AB_LOG_E("Failed to add email to database");
            ret = ErrorCode::kDbError;
            break;
        }
        result.second.SetRid(group_rid);
        trans_guard.Commit();
    } while (false);
    return result;
}

ErrorCode EmailService::RemoveEmail(const std::vector<uint32_t>& rids)
{
    ErrorCode result = ErrorCode::kSuccess;
    do
    {
        if (rids.empty())
        {
            AB_LOG_E("Email rids is empty");
            result = ErrorCode::kInvalidParam;
            break;
        }
        if (!m_mail_rep_sptr)
        {
            AB_LOG_E("Email repository is null");
            result = ErrorCode::kNotable;
            break;
        }
        TransactionGuard trans_guard;
        if (!m_mail_rep_sptr->RemoveEmail(rids))
        {
            AB_LOG_E("Failed to remove email from database");
            result = ErrorCode::kDbError;
            break;
        }
        trans_guard.Commit();
    } while (false);

    return result;
}

ErrorCode EmailService::RemoveGroup(const std::vector<uint32_t>& rids)
{
    ErrorCode result = ErrorCode::kSuccess;
    do
    {
        if (rids.empty())
        {
            AB_LOG_E("Group rids is empty");
            result = ErrorCode::kInvalidParam;
            break;
        }
        if (!m_mail_rep_sptr)
        {
            AB_LOG_E("Group repository is null");
            result = ErrorCode::kNotable;
            break;
        }
        TransactionGuard trans_guard;
        if (!m_mail_rep_sptr->RemoveGroup(rids))
        {
            AB_LOG_E("Failed to remove group from database");
            result = ErrorCode::kDbError;
            break;
        }
        trans_guard.Commit();
    } while (false);

    return result;
}

ErrorCode EmailService::UpdateEmail(const EmailDto& dto)
{
    ErrorCode result = ErrorCode::kSuccess;
    do
    {
        if (!m_mail_rep_sptr)
        {
            AB_LOG_E("Email repository is null");
            result = ErrorCode::kNotable;
            break;
        }
        // 输入合法验证
        if (dto.GetName().empty() || dto.GetAddress().empty())
        {
            AB_LOG_E("Invalid email name or address");
            result = ErrorCode::kInvalidParam;
            break;
        }
        // 检查邮件组数量是否超了规格上限
        std::vector<uint32_t> new_group_rids = dto.GetGroupRids();
        if (new_group_rids.size() > kMaxGroupsPerEmail)
        {
            AB_LOG_E("Exceed max group count");
            result = ErrorCode::kExceedMaxCount;
            break;
        }
        TransactionGuard trans_guard;
        // 删除旧关系
        if (!m_mail_rep_sptr->RemoveGroupByMailRid(dto.GetRid()))
        {
            AB_LOG_E("Failed to remove group from database");
            result = ErrorCode::kDbError;
            break;
        }
        // 检查邮件组是否存在
        bool group_exist = m_mail_rep_sptr->IsGroupExist(new_group_rids);
        if (!group_exist)
        {
            AB_LOG_E("Some groups do not exist");
            result = ErrorCode::kInvalidParam;
            break;
        }
        // 检查邮件组中是否还能关联邮件
        if (!m_mail_rep_sptr->CanAddEmail(new_group_rids, kMaxGroupsPerEmail))
        {
            AB_LOG_E("Some groups exceed max email count");
            result = ErrorCode::kExceedMaxCount;
            break;
        }
        // 更新邮件
        std::shared_ptr<EmailEntity> mail_entity_sptr = std::make_shared<EmailEntity>(dto.GetRid(), dto.GetName(), dto.GetAddress());
        if (!m_mail_rep_sptr->UpdateEmail(mail_entity_sptr, new_group_rids))
        {
            AB_LOG_E("Failed to update email to database");
            result = ErrorCode::kDbError;
            break;
        }
        trans_guard.Commit();
    } while (false);
    return result;
}

ErrorCode EmailService::UpdateGroup(const GroupDto& dto)
{
    return {};
}

std::pair<ErrorCode, SearchResult> EmailService::SearchEmail(const std::string& keyword, uint32_t current_page, uint32_t page_size)
{
    std::pair<ErrorCode, SearchResult> ret = std::make_pair(ErrorCode::kSuccess, SearchResult(current_page, page_size));
    ErrorCode& code = ret.first;
    SearchResult& result = ret.second;
    if (m_mail_rep_sptr)
    {
        PageResult page_result = m_mail_rep_sptr->GetEmailsByKeyword(keyword, current_page, page_size);
        std::vector<EmailDto> dtos;
        const std::vector<std::shared_ptr<AbstractEntity>>& items = page_result.GetRecords();
        std::transform(items.begin(), items.end(), std::back_inserter(dtos), [](const std::shared_ptr<AbstractEntity>& item) {
            std::shared_ptr<EmailEntity> mail_entity_sptr = std::static_pointer_cast<EmailEntity>(item);
            if (mail_entity_sptr)
            {
                uint32_t rid = mail_entity_sptr->GetRid();
                const std::string& email_address = mail_entity_sptr->GetEmailAddress();
                const std::string& email_name = mail_entity_sptr->GetEmailName();
                const std::string& group_rids = mail_entity_sptr->GetGroupRids();
                std::vector<uint32_t> rids = AddressCenterUtilities::ConvertToNumbers(AddressCenterUtilities::Split(group_rids, ","));
                std::vector<std::string> group_names = AddressCenterUtilities::Split(mail_entity_sptr->GetGroupNames(), "|##|");
                return EmailDto(rid, email_address, email_name, rids, group_names);
            }
            return EmailDto();
        });
        result.SetTotalRecords(page_result.GetTotalRecords());
        result.SetRecords(dtos);
    }
    else
    {
        code = ErrorCode::kDbError;
    }
    return ret;
}