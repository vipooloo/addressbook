#include "AddrCenter.h"
#include "EmailDao.h"
#include "EmailRepository.h"
#include "GroupDao.h"
#include "MailGroupDao.h"
#include <algorithm>

EmailRepository::EmailRepository()
  : m_mail_dao_sptr{std::make_shared<EmailDao>()}
  , m_group_dao_sptr{std::make_shared<GroupDao>()}
  , m_mail_group_dao_sptr{std::make_shared<MailGroupDao>()}
{
    if (m_mail_dao_sptr)
    {
        m_mail_dao_sptr->Init();
    }
    if (m_group_dao_sptr)
    {
        m_group_dao_sptr->Init();
    }
    if (m_mail_group_dao_sptr)
    {
        m_mail_group_dao_sptr->Init();
    }
}

uint32_t EmailRepository::AddEmail(const std::shared_ptr<EmailEntity>& entity_sptr, const std::vector<uint32_t>& rids)
{
    uint32_t rid = 0;
    if (m_mail_dao_sptr && entity_sptr)
    {
        std::shared_ptr<AbstractEntity> out_entity_sptr = std::make_shared<EmailEntity>();
        bool result = m_mail_dao_sptr->Insert(entity_sptr, out_entity_sptr);
        if (out_entity_sptr && result)
        {
            uint32_t email_rid = out_entity_sptr->GetRid();
            // 添加邮件到组的映射关系
            std::vector<std::shared_ptr<AbstractEntity>> relations;
            relations.reserve(rids.size());
            std::transform(
                rids.cbegin(),
                rids.cend(),
                std::back_inserter(relations),
                [email_rid](uint32_t group_rid) {
                    return std::make_shared<MailGroupRelation>(email_rid, group_rid);
                });
            if (AddEmailToGroupRelation(relations))
            {
                rid = out_entity_sptr->GetRid();
            }
        }
    }
    return rid;
}

uint32_t EmailRepository::AddGroup(const std::shared_ptr<GroupEntity>& entity_sptr, const std::vector<uint32_t>& rids)
{
    uint32_t rid = 0;
    if (m_mail_dao_sptr && entity_sptr)
    {
        std::shared_ptr<AbstractEntity> out_entity_sptr = std::make_shared<GroupEntity>();
        bool result = m_group_dao_sptr->Insert(entity_sptr, out_entity_sptr);
        if (out_entity_sptr && result)
        {
            uint32_t group_rid = out_entity_sptr->GetRid();
            // 添加邮件到组的映射关系
            std::vector<std::shared_ptr<AbstractEntity>> relations;
            relations.reserve(rids.size());
            std::transform(
                rids.cbegin(),
                rids.cend(),
                std::back_inserter(relations),
                [group_rid](uint32_t email_rid) {
                    return std::make_shared<MailGroupRelation>(email_rid, group_rid);
                });
            if (AddEmailToGroupRelation(relations))
            {
                rid = out_entity_sptr->GetRid();
            }
        }
    }
    return rid;
}

size_t EmailRepository::GetGroupCount() const
{
    size_t count = 0;
    if (m_group_dao_sptr)
    {
        count = m_group_dao_sptr->GetCount();
    }
    return count;
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

bool EmailRepository::CanAddGroup(const std::vector<uint32_t>& group_ids, uint32_t count_limit)
{
    bool result = false;
    if (m_mail_group_dao_sptr)
    {
        result = m_mail_group_dao_sptr->HasMemberOverGroupLimit(group_ids, count_limit);
    }
    return result;
}

bool EmailRepository::CanAddEmail(const std::vector<uint32_t>& group_ids, uint32_t count_limit)
{
    bool result = false;
    if (m_mail_group_dao_sptr)
    {
        result = m_mail_group_dao_sptr->HasMemberOverEMailLimit(group_ids, count_limit);
    }
    return result;
}

bool EmailRepository::AddEmailToGroupRelation(const std::vector<std::shared_ptr<AbstractEntity>>& items)
{
    bool result = false;
    if (m_mail_dao_sptr)
    {
        result = m_mail_group_dao_sptr->InsertBatch(items);
    }
    return result;
}

bool EmailRepository::IsGroupExist(const std::vector<uint32_t>& group_ids) const
{
    bool result = false;
    if (group_ids.empty())
    {
        result = true;
    }
    else
    {
        if (m_group_dao_sptr)
        {
            result = m_group_dao_sptr->IsExist(group_ids);
        }
    }
    return result;
}

bool EmailRepository::IsMailExist(const std::vector<uint32_t>& mail_ids) const
{
    bool result = false;
    if (mail_ids.empty())
    {
        result = true;
    }
    else
    {
        if (m_mail_dao_sptr)
        {
            result = m_mail_dao_sptr->IsExist(mail_ids);
        }
    }
    return result;
}

bool EmailRepository::RemoveEmail(const std::vector<uint32_t>& mail_ids)
{
    bool result = false;
    if (mail_ids.empty())
    {
        result = true;
    }
    else
    {
        if (m_mail_dao_sptr)
        {
            result = m_mail_dao_sptr->Remove(mail_ids);
        }
    }
    return result;
}

bool EmailRepository::RemoveGroup(const std::vector<uint32_t>& group_ids)
{
    bool result = false;
    if (group_ids.empty())
    {
        result = true;
    }
    else
    {
        if (m_group_dao_sptr)
        {
            result = m_group_dao_sptr->Remove(group_ids);
        }
    }
    return result;
}

bool EmailRepository::UpdateEmail(const std::shared_ptr<EmailEntity>& entity_sptr, const std::vector<uint32_t>& add_group_ids, const std::vector<uint32_t>& remove_group_ids)
{
    bool result = false;
    if (m_mail_dao_sptr && entity_sptr)
    {
        result = m_mail_dao_sptr->Update(entity_sptr);
    }
    return result;
}

bool EmailRepository::UpdateGroup(const std::shared_ptr<GroupEntity>& entity_sptr)
{
    return false;
}

std::vector<uint32_t> EmailRepository::GetGroupRidsByMailRid(uint32_t mail_rid)
{
    std::vector<uint32_t> ret;
    if (m_mail_group_dao_sptr)
    {
        ret = m_mail_group_dao_sptr->GetEmailGroupsByEmailId(mail_rid);
    }
    return ret;
}