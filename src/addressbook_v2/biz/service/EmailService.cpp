#include "AddrMgrConfigDefs.h"
#include "AddrMgrLog.h"
#include "AddrMgrUtilities.h"
#include "CheckerProvider.h"
#include "EmailEntity.h"
#include "EmailRepository.h"
#include "EmailService.h"
#include "PageResult.h"
#include "TransactionGuard.h"
#include <algorithm>

namespace addrbook {
EmailService::EmailService(EventDispatcher& dispatcher)
  : m_env_dispatcher{dispatcher}
  , m_mail_rep_sptr{std::make_shared<EmailRepository>()}
{
}

std::pair<ResultCode, EmailDto> EmailService::AddEmail(const EmailDto& dto)
{
    std::pair<ResultCode, EmailDto> result = std::make_pair(ResultCode::kSuccess, dto);
    ResultCode& res_code = result.first;
    do
    {
        if (!m_mail_rep_sptr)
        {
            AB_LOG_E("Email repository is null");
            res_code = ResultCode::kNotable;
            break;
        }
        // 输入合法验证
        if (dto.GetName().empty() || dto.GetAddress().empty())
        {
            AB_LOG_E("Invalid email name or address");
            res_code = ResultCode::kInvalidParam;
            break;
        }
        std::vector<uint32_t> group_rids = dto.GetGroupRids();
        // 检查其中是否还有rid=0
        if (std::any_of(group_rids.cbegin(), group_rids.cend(), [](uint32_t rid) {
                return rid == 0;
            }))
        {
            AB_LOG_E("Invalid group rid");
            res_code = ResultCode::kInvalidParam;
            break;
        }
        // 检查邮件组数量是否超了规格上限
        if (group_rids.size() > kMaxGroupsPerEmail)
        {
            AB_LOG_E("Exceed max group count");
            res_code = ResultCode::kExceedMaxCount;
            break;
        }
        TransactionGuard trans_guard;
        // 检查邮件是否超过最大数量限制
        if (m_mail_rep_sptr->GetEmailCount() >= kMaxMailCount)
        {
            AB_LOG_E("Exceed max email count");
            res_code = ResultCode::kExceedMaxCount;
            break;
        }
        // 检查邮件组是否存在
        bool group_exist = m_mail_rep_sptr->IsGroupExist(group_rids);
        if (!group_exist)
        {
            AB_LOG_E("Some groups do not exist");
            res_code = ResultCode::kInvalidParam;
            break;
        }
        // 检查邮件组中是否还能关联邮件
        if (!m_mail_rep_sptr->CanAddEmail(group_rids, kMaxGroupsPerEmail))
        {
            AB_LOG_E("Some groups exceed max email count");
            res_code = ResultCode::kExceedMaxCount;
            break;
        }
        // 添加邮件
        std::shared_ptr<EmailEntity> mail_entity_sptr = std::make_shared<EmailEntity>(dto.GetName(), dto.GetAddress());
        uint32_t rid = m_mail_rep_sptr->AddEmail(mail_entity_sptr, group_rids);
        if (0 == rid)
        {
            AB_LOG_E("Failed to add email to database");
            res_code = ResultCode::kDbError;
            break;
        }
        result.second.SetRid(rid);
        trans_guard.Commit();
    } while (false);
    DataChanged(res_code, ChangeType::kAddEmail);
    return result;
}

std::pair<ResultCode, GroupDto> EmailService::AddGroup(const GroupDto& dto)
{
    std::pair<ResultCode, GroupDto> result = std::make_pair(ResultCode::kSuccess, dto);
    ResultCode& res_code = result.first;
    do
    {
        if (!m_mail_rep_sptr)
        {
            AB_LOG_E("Email repository is null");
            res_code = ResultCode::kNotable;
            break;
        }
        // 输入合法验证
        if (dto.GetGroupName().empty())
        {
            AB_LOG_E("Invalid group name");
            res_code = ResultCode::kInvalidParam;
            break;
        }
        std::vector<uint32_t> mail_rids = dto.GetMailRids();
        // 检查其中是否还有rid=0
        if (std::any_of(mail_rids.cbegin(), mail_rids.cend(), [](uint32_t rid) {
                return rid == 0;
            }))
        {
            AB_LOG_E("Invalid mail rid");
            res_code = ResultCode::kInvalidParam;
            break;
        }
        // 检查邮件数量是否超了规格上限
        if (mail_rids.size() > kMaxEmailsPerGroup)
        {
            AB_LOG_E("Exceed max email count");
            res_code = ResultCode::kExceedMaxCount;
            break;
        }
        TransactionGuard trans_guard;
        // 检查邮件组是否超过最大数量限制
        if (m_mail_rep_sptr->GetGroupCount() >= kMaxGroupCount)
        {
            AB_LOG_E("Exceed max group count");
            res_code = ResultCode::kExceedMaxCount;
            break;
        }
        // 检查邮件是否存在
        bool group_exist = m_mail_rep_sptr->IsMailExist(mail_rids);
        if (!group_exist)
        {
            AB_LOG_E("Some mails do not exist");
            res_code = ResultCode::kInvalidParam;
            break;
        }
        // 检查邮件是否还能关联邮件组
        if (!m_mail_rep_sptr->CanAddGroup(mail_rids, kMaxEmailsPerGroup))
        {
            AB_LOG_E("Some mails exceed max email count");
            res_code = ResultCode::kExceedMaxCount;
            break;
        }
        // 添加邮件组
        std::shared_ptr<GroupEntity> group_entity_sptr = std::make_shared<GroupEntity>(dto.GetGroupName());
        uint32_t group_rid = m_mail_rep_sptr->AddGroup(group_entity_sptr, mail_rids);
        if (0 == group_rid)
        {
            AB_LOG_E("Failed to add email to database");
            res_code = ResultCode::kDbError;
            break;
        }
        result.second.SetRid(group_rid);
        trans_guard.Commit();
    } while (false);
    return result;
}

ResultCode EmailService::RemoveEmail(const std::vector<uint32_t>& rids)
{
    ResultCode result = ResultCode::kSuccess;
    do
    {
        if (rids.empty())
        {
            AB_LOG_E("Email rids is empty");
            result = ResultCode::kInvalidParam;
            break;
        }
        if (!m_mail_rep_sptr)
        {
            AB_LOG_E("Email repository is null");
            result = ResultCode::kNotable;
            break;
        }
        TransactionGuard trans_guard;
        if (!m_mail_rep_sptr->RemoveEmail(rids))
        {
            AB_LOG_E("Failed to remove email from database");
            result = ResultCode::kDbError;
            break;
        }
        trans_guard.Commit();
    } while (false);

    return result;
}

ResultCode EmailService::RemoveGroup(const std::vector<uint32_t>& rids)
{
    ResultCode result = ResultCode::kSuccess;
    do
    {
        if (rids.empty())
        {
            AB_LOG_E("Group rids is empty");
            result = ResultCode::kInvalidParam;
            break;
        }
        if (!m_mail_rep_sptr)
        {
            AB_LOG_E("Group repository is null");
            result = ResultCode::kNotable;
            break;
        }
        TransactionGuard trans_guard;
        if (!m_mail_rep_sptr->RemoveGroup(rids))
        {
            AB_LOG_E("Failed to remove group from database");
            result = ResultCode::kDbError;
            break;
        }
        trans_guard.Commit();
    } while (false);

    return result;
}

ResultCode EmailService::UpdateEmail(const EmailDto& dto)
{
    ResultCode result = ResultCode::kSuccess;
    do
    {
        if (!m_mail_rep_sptr)
        {
            AB_LOG_E("Email repository is null");
            result = ResultCode::kNotable;
            break;
        }
        // 输入合法验证
        if (dto.GetName().empty() || dto.GetAddress().empty())
        {
            AB_LOG_E("Invalid email name or address");
            result = ResultCode::kInvalidParam;
            break;
        }
        // 检查邮件组数量是否超了规格上限
        std::vector<uint32_t> new_group_rids = dto.GetGroupRids();
        if (new_group_rids.size() > kMaxGroupsPerEmail)
        {
            AB_LOG_E("Exceed max group count");
            result = ResultCode::kExceedMaxCount;
            break;
        }
        TransactionGuard trans_guard;
        // 删除旧关系
        if (!m_mail_rep_sptr->RemoveGroupByMailRid(dto.GetRid()))
        {
            AB_LOG_E("Failed to remove group from database");
            result = ResultCode::kDbError;
            break;
        }
        // 检查邮件组是否存在
        bool group_exist = m_mail_rep_sptr->IsGroupExist(new_group_rids);
        if (!group_exist)
        {
            AB_LOG_E("Some groups do not exist");
            result = ResultCode::kInvalidParam;
            break;
        }
        // 检查邮件组中是否还能关联邮件
        if (!m_mail_rep_sptr->CanAddEmail(new_group_rids, kMaxGroupsPerEmail))
        {
            AB_LOG_E("Some groups exceed max email count");
            result = ResultCode::kExceedMaxCount;
            break;
        }
        // 更新邮件
        std::shared_ptr<EmailEntity> mail_entity_sptr = std::make_shared<EmailEntity>(dto.GetRid(), dto.GetName(), dto.GetAddress());
        if (!m_mail_rep_sptr->UpdateEmail(mail_entity_sptr, new_group_rids))
        {
            AB_LOG_E("Failed to update email to database");
            result = ResultCode::kDbError;
            break;
        }
        trans_guard.Commit();
    } while (false);
    return result;
}

ResultCode EmailService::UpdateGroup(const GroupDto& dto)
{
    static_cast<void>(&dto);
    return {};
}

std::pair<ResultCode, SearchEmailResult> EmailService::SearchEmail(const std::string& keyword, uint32_t current_page, uint32_t page_size)
{
    std::pair<ResultCode, SearchEmailResult> ret = std::make_pair(ResultCode::kSuccess, SearchEmailResult(current_page, page_size));
    ResultCode& code = ret.first;
    SearchEmailResult& result = ret.second;
    if (m_mail_rep_sptr)
    {
        TransactionGuard trans_guard;
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
                std::vector<uint32_t> rids = AddrMgrUtilities::ConvertToNumbers(AddrMgrUtilities::Split(group_rids, ","));
                std::vector<std::string> group_names = AddrMgrUtilities::Split(mail_entity_sptr->GetGroupNames(), "|##|");
                return EmailDto(rid, email_address, email_name, rids, group_names);
            }
            return EmailDto();
        });
        trans_guard.Commit();
        result.SetTotalRecords(page_result.GetTotalRecords());
        result.SetRecords(dtos);
    }
    else
    {
        code = ResultCode::kDbError;
    }
    return ret;
}

void EmailService::DataChanged(ResultCode res, ChangeType type)
{
    if (ResultCode::kSuccess == res)
    {
        m_env_dispatcher.Notify(type);
    }
}

}  // namespace addrbook