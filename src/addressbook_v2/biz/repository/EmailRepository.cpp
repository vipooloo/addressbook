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

bool EmailRepository::AddEmail(const std::shared_ptr<EmailEntity>& entity_sptr, std::shared_ptr<AbstractEntity>& out_entity_sptr)
{
    bool result = false;
    if (m_mail_dao_sptr && entity_sptr)
    {
        result = m_mail_dao_sptr->Insert(entity_sptr, out_entity_sptr);
    }
    return result;
}

bool EmailRepository::AddGroup(const std::shared_ptr<GroupEntity>& entity_sptr, std::shared_ptr<AbstractEntity>& out_entity_sptr)
{
    bool result = false;
    if (m_group_dao_sptr && entity_sptr)
    {
        result = m_group_dao_sptr->Insert(entity_sptr, out_entity_sptr);
    }
    return result;
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
        std::map<uint32_t, uint32_t> mail_counts = m_mail_group_dao_sptr->GetGroupEmailCounts(group_ids, false);
        result = std::all_of(mail_counts.cbegin(), mail_counts.cend(), [count_limit](const std::pair<uint32_t, uint32_t>& item) {
            return item.second < count_limit;
        });
    }
    return result;
}

bool EmailRepository::CanAddEmail(const std::vector<uint32_t>& mail_ids, uint32_t count_limit)
{
    bool result = false;
    if (m_mail_group_dao_sptr)
    {
        std::map<uint32_t, uint32_t> mail_counts = m_mail_group_dao_sptr->GetGroupEmailCounts(mail_ids, true);
        result = std::all_of(mail_counts.cbegin(), mail_counts.cend(), [count_limit](const std::pair<uint32_t, uint32_t>& item) {
            return item.second < count_limit;
        });
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

bool EmailRepository::UpdateEmail(const std::shared_ptr<EmailEntity>& entity_sptr)
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