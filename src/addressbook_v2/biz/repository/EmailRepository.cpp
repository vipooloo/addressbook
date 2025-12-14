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

bool EmailRepository::AddEmail(const EmailEntity& entity, std::shared_ptr<AbstractEntity>& out_entity_sptr)
{
    bool result = false;
    if (m_mail_dao_sptr)
    {
        result = m_mail_dao_sptr->Insert(entity, out_entity_sptr);
    }
    return result;
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

bool EmailRepository::AddGroup(const GroupEntity& entity)
{
    bool result = false;
    if (m_group_dao_sptr)
    {
        auto s = std::shared_ptr<AbstractEntity>();
        result = m_group_dao_sptr->Insert(entity, s);
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
            result = (DaoErrCode::SUCCESS == m_group_dao_sptr->IsExist(group_ids));
        }
    }
    return result;
}

bool EmailRepository::IsGroupCanAddEmail(const std::vector<uint32_t>& group_ids, uint32_t add_count)
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

bool EmailRepository::AddEmailToGroupRelation(const MailGroupRelation& entity)
{
    bool result = false;
    if (m_mail_dao_sptr)
    {
        std::shared_ptr<AbstractEntity> out_entity_sptr = nullptr;
        result = m_mail_group_dao_sptr->Insert(entity, out_entity_sptr);
    }
    return result;
}