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

uint32_t EmailRepository::CreateEmail(const EmailEntity& entity, const std::vector<uint32_t>& ids)
{
    uint32_t rid = 0;
    std::pair<bool, EmailEntity> result = m_mail_dao.Insert(entity);
    if (result.first)
    {
        uint32_t email_rid = result.second.GetRid();
        std::vector<EmailGroupEntity> relations;
        relations.reserve(ids.size());
        std::transform(
            ids.cbegin(),
            ids.cend(),
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

uint32_t EmailRepository::GetOrCreateEmail(const EmailEntity& entity)
{
    uint32_t rid = m_mail_dao.GetEmailRid(entity);
    if (0 == rid)
    {
        rid = CreateEmail(entity, {});
    }
    return rid;
}

uint32_t EmailRepository::CreateGroup(const GroupEntity& entity, const std::vector<uint32_t>& ids)
{
    uint32_t rid = 0;

    std::pair<bool, GroupEntity> result = m_group_dao.Insert(entity);
    if (result.first)
    {
        uint32_t group_rid = result.second.GetRid();
        std::vector<EmailGroupEntity> relations;
        relations.reserve(ids.size());
        std::transform(
            ids.cbegin(),
            ids.cend(),
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

uint32_t EmailRepository::GetOrCreateGroup(const GroupEntity& entity, uint32_t email_rid)
{
    uint32_t rid = 0;
    do
    {
        rid = m_group_dao.GetGroupRid(entity);
        if (0 == rid)
        {
            rid = CreateGroup(entity, {email_rid});
        }
        if (0 == rid)
        {
            // 添加组失败
            break;
        }
        if (!m_mail_group_dao.InsertBatch({EmailGroupEntity(email_rid, rid)}))
        {
            // 添加映射关系失败
            rid = 0;
            break;
        }
    } while (false);
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

bool EmailRepository::CanAddEmail(const std::vector<uint32_t>& email_ids, uint32_t count_limit)
{
    return m_mail_group_dao.HasMemberOverEMailLimit(email_ids, count_limit);
}

bool EmailRepository::CanAddGroup(const std::vector<uint32_t>& group_ids, uint32_t count_limit)
{
    return m_mail_group_dao.HasMemberOverGroupLimit(group_ids, count_limit);
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

bool EmailRepository::IsEmailExist(const std::vector<uint32_t>& email_ids) const
{
    bool result = true;
    if (!email_ids.empty())
    {
        result = m_mail_dao.IsExist(email_ids);
    }
    return result;
}

bool EmailRepository::DeleteEmails(const std::vector<uint32_t>& email_ids)
{
    bool result = true;
    if (!email_ids.empty())
    {
        result = m_mail_dao.Remove(email_ids);
    }
    return result;
}

bool EmailRepository::DeleteGroup(const std::vector<uint32_t>& group_ids)
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
    // 更新关联邮件组信息
    if (result && !new_group_rids.empty())
    {
        uint32_t email_rid = entity.GetRid();
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

bool EmailRepository::UpdateGroup(const GroupEntity& entity, const std::vector<uint32_t>& new_mail_rids)
{
    bool result = true;

    // 更新邮件表信息
    result = m_group_dao.Update(entity);
    // 更新关联邮件组信息
    if (result && !new_mail_rids.empty())
    {
        uint32_t group_rid = entity.GetRid();
        std::vector<EmailGroupEntity> relations;
        relations.reserve(new_mail_rids.size());
        std::transform(
            new_mail_rids.cbegin(),
            new_mail_rids.cend(),
            std::back_inserter(relations),
            [group_rid](uint32_t mail_rid) {
                return EmailGroupEntity(mail_rid, group_rid);
            });
        if (!m_mail_group_dao.InsertBatch(relations))
        {
            result = false;
        }
    }

    return result;
}

bool EmailRepository::RemoveGroupByMailRid(uint32_t email_rid)
{
    return m_mail_group_dao.RemoveByEmailRid(email_rid);
}

PageResult EmailRepository::GetEmailsByKeyword(const PageQueryParam& query_param)
{
    PageResult result(query_param.GetCurPage(), query_param.GetPageSize());

    if (query_param.GetKeyword().empty())
    {
        result = m_mail_dao.FindAll(query_param);
    }
    else
    {
        result = m_mail_dao.FindByPage(query_param);
    }

    return result;
}

PageResult EmailRepository::GetGroupsByKeyword(const PageQueryParam& query_param)
{
    PageResult result(query_param.GetCurPage(), query_param.GetPageSize());

    if (query_param.GetKeyword().empty())
    {
        result = m_group_dao.FindAll(query_param);
    }
    else
    {
        result = m_group_dao.FindByPage(query_param);
    }

    return result;
}

bool EmailRepository::DeleteAllEmails()
{
    return m_mail_dao.RemoveAll();
}

bool EmailRepository::DeleteAllGroups()
{
    return m_group_dao.RemoveAll();
}
}  // namespace addrbook