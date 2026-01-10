#include "EmailDao.h"
#include "EmailGroupDao.h"
#include "EmailRepository.h"
#include "GroupDao.h"
#include <algorithm>

namespace addrbook {
EmailRepository::EmailRepository()
  : m_mail_dao{}
  , m_group_dao{}
  , m_mail_group_dao{}
{
    m_mail_dao.Create();
    m_group_dao.Create();
    m_mail_group_dao.Create();
}

uint32_t EmailRepository::AddEmail(const EmailEntity& entity, const std::vector<uint32_t>& rids)
{
    uint32_t rid = 0;
    std::pair<bool, EmailEntity> result = m_mail_dao.Insert(entity);
    if (result.first)
    {
        uint32_t email_rid = result.second.GetRid();
        std::vector<EmailGroupEntity> relations;
        relations.reserve(rids.size());
        std::transform(
            rids.cbegin(),
            rids.cend(),
            std::back_inserter(relations),
            [email_rid](uint32_t group_rid) {
                return EmailGroupEntity(email_rid, group_rid);
            });
        if (m_mail_group_dao.InsertBatch(relations))
        {
            rid = email_rid;
        }
    }
    return rid;
}

uint32_t EmailRepository::AddGroup(const GroupEntity& entity, const std::vector<uint32_t>& rids)
{
    uint32_t rid = 0;

    std::pair<bool, GroupEntity> result = m_group_dao.Insert(entity);
    if (result.first)
    {
        uint32_t group_rid = result.second.GetRid();
        std::vector<EmailGroupEntity> relations;
        relations.reserve(rids.size());
        std::transform(
            rids.cbegin(),
            rids.cend(),
            std::back_inserter(relations),
            [group_rid](uint32_t email_rid) {
                return EmailGroupEntity(email_rid, group_rid);
            });
        if (m_mail_group_dao.InsertBatch(relations))
        {
            rid = group_rid;
        }
    }

    return rid;
}

size_t EmailRepository::GetGroupCount() const
{
    return m_group_dao.GetCount();
}

uint32_t EmailRepository::GetEmailCount() const
{
    return m_mail_dao.GetCount();
}

bool EmailRepository::CanAddGroup(const std::vector<uint32_t>& group_ids, uint32_t count_limit)
{
    return m_mail_group_dao.HasMemberOverGroupLimit(group_ids, count_limit);
}

bool EmailRepository::CanAddEmail(const std::vector<uint32_t>& group_ids, uint32_t count_limit)
{
    return m_mail_group_dao.HasMemberOverEMailLimit(group_ids, count_limit);
}

bool EmailRepository::IsGroupExist(const std::vector<uint32_t>& group_ids) const
{
    bool result = true;
    if (!group_ids.empty())
    {
        result = m_group_dao.IsExist(group_ids);
    }
    return result;
}

bool EmailRepository::IsMailExist(const std::vector<uint32_t>& mail_ids) const
{
    bool result = true;
    if (!mail_ids.empty())
    {
        result = m_mail_dao.IsExist(mail_ids);
    }
    return result;
}

bool EmailRepository::RemoveEmail(const std::vector<uint32_t>& mail_ids)
{
    bool result = true;
    if (!mail_ids.empty())
    {
        result = m_mail_dao.Remove(mail_ids);
    }
    return result;
}

bool EmailRepository::RemoveGroup(const std::vector<uint32_t>& group_ids)
{
    bool result = true;
    if (!group_ids.empty())
    {
        result = m_group_dao.Remove(group_ids);
    }
    return result;
}

bool EmailRepository::UpdateEmail(const EmailEntity& entity, const std::vector<uint32_t>& new_group_rids)
{
    bool result = true;

    // 更新邮件表信息
    result = m_mail_dao.Update(entity);
    if (result && !new_group_rids.empty())
    {
        // 映射表中增加信息
        uint32_t email_rid = entity.GetRid();
        // 添加邮件到组的映射关系
        std::vector<EmailGroupEntity> relations;
        relations.reserve(new_group_rids.size());
        std::transform(
            new_group_rids.cbegin(),
            new_group_rids.cend(),
            std::back_inserter(relations),
            [email_rid](uint32_t group_rid) {
                return EmailGroupEntity(email_rid, group_rid);
            });
        if (!m_mail_group_dao.InsertBatch(relations))
        {
            result = false;
        }
    }
    return result;
}

bool EmailRepository::UpdateGroup(const std::shared_ptr<GroupEntity>& entity_sptr)
{
    static_cast<void>(entity_sptr);
    return false;
}

bool EmailRepository::RemoveGroupByMailRid(uint32_t mail_rid)
{
    bool ret = false;
    if (mail_rid != 0)
    {
        // TBD 这个判断是不是应该在输入的地方检查呢
        ret = m_mail_group_dao.RemoveByEmailRid(mail_rid);
    }
    return ret;
}

PageResult EmailRepository::GetEmailsByKeyword(const std::string& keyword, uint32_t page_num, uint32_t page_size)
{
    PageResult result(page_num, page_size);

    if (keyword.empty())
    {
        result = m_mail_dao.FindAll(page_num, page_size);
    }
    else
    {
        result = m_mail_dao.FindByPage(keyword, page_num, page_size);
    }

    return result;
}
}  // namespace addrbook