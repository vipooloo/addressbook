#include "AddrMgrConfigDefs.h"
#include "AddrMgrLog.h"
#include "AddrMgrUtilities.h"
#include "EmailEntity.h"
#include "EmailService.h"
#include "PageResult.h"
#include "TransactionGuard.h"
#include <algorithm>

namespace addrbook {
EmailService::EmailService(EventDispatcher& dispatcher)
  : m_env_dispatcher{dispatcher}
  , m_mail_rep{}
{
}

std::pair<ResultCode, EmailDto> EmailService::AddEmail(const EmailDto& dto)
{
    std::pair<ResultCode, EmailDto> result = std::make_pair(ResultCode::kSuccess, dto);
    do
    {
        TransactionGuard trans_guard;
        // 检查邮件是否超过最大数量限制
        if (m_mail_rep.GetEmailCount() >= kMaxMailCount)
        {
            AB_LOG_E("Exceed max email count");
            result.first = ResultCode::kExceedMaxCount;
            break;
        }
        // 检查邮件组是否存在
        std::vector<uint32_t> group_rids = dto.GetGroupRids();
        bool group_exist = m_mail_rep.IsGroupExist(group_rids);
        if (!group_exist)
        {
            AB_LOG_E("Some groups do not exist");
            result.first = ResultCode::kInvalidParam;
            break;
        }
        // 检查邮件组中是否还能关联邮件
        if (!m_mail_rep.CanAddEmail(group_rids, kMaxGroupsPerEmail))
        {
            AB_LOG_E("Some groups exceed max email count");
            result.first = ResultCode::kExceedMaxCount;
            break;
        }
        // 添加邮件
        EmailEntity entity = EmailEntity(dto.GetName(), dto.GetAddress());
        uint32_t rid = m_mail_rep.AddEmail(entity, group_rids);
        if (0 == rid)
        {
            AB_LOG_E("Failed to add email to database");
            result.first = ResultCode::kDbError;
            break;
        }
        result.second.SetRid(rid);
        trans_guard.Commit();
    } while (false);
    DataChanged(result.first, ChangeType::kAddEmail);
    return result;
}

std::pair<ResultCode, GroupDto> EmailService::AddGroup(const GroupDto& dto)
{
    std::pair<ResultCode, GroupDto> result = std::make_pair(ResultCode::kSuccess, dto);
    ResultCode& res_code = result.first;
    do
    {
        TransactionGuard trans_guard;
        // 检查邮件组是否超过最大数量限制
        if (m_mail_rep.GetGroupCount() >= kMaxGroupCount)
        {
            AB_LOG_E("Exceed max group count");
            res_code = ResultCode::kExceedMaxCount;
            break;
        }
        // 检查邮件是否存在
        std::vector<uint32_t> mail_rids = dto.GetMailRids();
        bool group_exist = m_mail_rep.IsMailExist(mail_rids);
        if (!group_exist)
        {
            AB_LOG_E("Some mails do not exist");
            res_code = ResultCode::kInvalidParam;
            break;
        }
        // 检查邮件是否还能关联邮件组
        if (!m_mail_rep.CanAddGroup(mail_rids, kMaxEmailsPerGroup))
        {
            AB_LOG_E("Some mails exceed max email count");
            res_code = ResultCode::kExceedMaxCount;
            break;
        }
        // 添加邮件组
        GroupEntity entity(dto.GetGroupName());
        uint32_t group_rid = m_mail_rep.AddGroup(entity, mail_rids);
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

ResultCode EmailService::AddEmailAndGroup(const EmailDto& dto)
{
    ResultCode result = ResultCode::kSuccess;
    do
    {
        TransactionGuard trans_guard;
        // 检查邮件是否超过最大数量限制
        if (m_mail_rep.GetEmailCount() >= kMaxMailCount)
        {
            AB_LOG_E("Exceed max email count");
            result = ResultCode::kExceedMaxCount;
            break;
        }
        // 添加邮件
        EmailEntity entity = EmailEntity(dto.GetName(), dto.GetAddress());
        uint32_t email_rid = m_mail_rep.GetOrCreateEmail(entity);
        if (0 == email_rid)
        {
            AB_LOG_E("Failed to add email to database");
            result = ResultCode::kDbError;
            break;
        }
        // 添加邮件组
        const std::vector<std::string>& group_names = dto.GetGroupNames();
        for (const std::string& group_name : group_names)
        {
            // 检查邮件组是否超过最大数量限制
            if (m_mail_rep.GetGroupCount() >= kMaxGroupCount)
            {
                AB_LOG_E("Exceed max group count");
                result = ResultCode::kExceedMaxCount;
                break;
            }
            GroupEntity group_entity(group_name);
            uint32_t group_rid = m_mail_rep.GetOrCreateGroup(group_entity, email_rid);
            if (0 == group_rid)
            {
                AB_LOG_E("Failed to add group to database");
                result = ResultCode::kDbError;
                break;
            }
        }
        if (ResultCode::kSuccess == result)
        {
            trans_guard.Commit();
        }
    } while (false);
    return result;
}

ResultCode EmailService::RemoveEmail(const std::vector<uint32_t>& rids)
{
    ResultCode result = ResultCode::kSuccess;

    TransactionGuard trans_guard;
    if (m_mail_rep.RemoveEmail(rids))
    {
        trans_guard.Commit();
    }
    else
    {
        AB_LOG_E("Failed %s", __FUNCTION__);
        result = ResultCode::kDbError;
    }

    return result;
}

ResultCode EmailService::RemoveGroup(const std::vector<uint32_t>& rids)
{
    ResultCode result = ResultCode::kSuccess;

    TransactionGuard trans_guard;
    if (m_mail_rep.RemoveGroup(rids))
    {
        trans_guard.Commit();
    }
    else
    {
        AB_LOG_E("Failed %s", __FUNCTION__);
        result = ResultCode::kDbError;
    }

    return result;
}

ResultCode EmailService::UpdateEmail(const EmailDto& dto)
{
    ResultCode result = ResultCode::kSuccess;
    do
    {
        TransactionGuard trans_guard;
        // 删除旧关系
        if (!m_mail_rep.RemoveGroupByMailRid(dto.GetRid()))
        {
            AB_LOG_E("Failed to remove group by mail rid");
            result = ResultCode::kDbError;
            break;
        }
        // 检查邮件组是否存在
        std::vector<uint32_t> new_group_rids = dto.GetGroupRids();
        bool group_exist = m_mail_rep.IsGroupExist(new_group_rids);
        if (!group_exist)
        {
            AB_LOG_E("Some groups do not exist");
            result = ResultCode::kInvalidParam;
            break;
        }
        // 检查邮件组中是否还能关联邮件
        if (!m_mail_rep.CanAddEmail(new_group_rids, kMaxGroupsPerEmail))
        {
            AB_LOG_E("Some groups exceed max email count");
            result = ResultCode::kExceedMaxCount;
            break;
        }
        // 更新邮件
        EmailEntity entity = EmailEntity(dto.GetRid(), dto.GetName(), dto.GetAddress());
        if (!m_mail_rep.UpdateEmail(entity, new_group_rids))
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

std::pair<ResultCode, SearchEmailResult> EmailService::QueryEmail(const QueryParam& query_param)
{
    std::pair<ResultCode, SearchEmailResult> result = std::make_pair(ResultCode::kSuccess, SearchEmailResult(query_param.GetCurPage(), query_param.GetPageSize()));

    TransactionGuard trans_guard;
    PageResult page_result = m_mail_rep.GetEmailsByKeyword(query_param);
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
    result.second.SetTotalRecords(page_result.GetTotalRecords());
    result.second.SetRecords(dtos);

    return result;
}

void EmailService::DataChanged(ResultCode res, ChangeType type)
{
    if (ResultCode::kSuccess == res)
    {
        m_env_dispatcher.Notify(type);
    }
}

void EmailService::ClearAllEmails()
{
    TransactionGuard trans_guard;
    if (m_mail_rep.ClearAllEmails())
    {
        trans_guard.Commit();
    }
}
}  // namespace addrbook