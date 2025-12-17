#include "AddrCenter.h"
#include "EmailDao.h"
#include "EmailRepository.h"
#include "GroupDao.h"
#include "MailGroupDao.h"

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

bool EmailRepository::CanAddGroup(const std::vector<uint32_t>& group_ids, uint32_t add_count)
{
    bool result = true;
    if (m_mail_group_dao_sptr)
    {
        for (uint32_t group_id : group_ids)
        {
            MailGroupMappingQueryCond cond;
            cond.SetGroupRid(group_id);
            size_t count = m_mail_group_dao_sptr->CountByCond(cond);
            if (count >= add_count)
            {
                break;
            }
        }
    }
    else
    {
        result = false;
    }
    return result;
}

bool EmailRepository::CanAddEmail(const std::vector<uint32_t>& mail_ids, uint32_t add_count)
{
    bool result = true;
    if (m_mail_group_dao_sptr)
    {
        for (uint32_t mail_rid : mail_ids)
        {
            MailGroupMappingQueryCond cond;
            cond.SetMailRid(mail_rid);
            size_t count = m_mail_group_dao_sptr->CountByCond(cond);
            if (count >= add_count)
            {
                break;
            }
        }
    }
    else
    {
        result = false;
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