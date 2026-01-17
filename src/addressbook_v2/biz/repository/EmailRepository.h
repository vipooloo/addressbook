#ifndef EMAILREPOSITORY_H
#define EMAILREPOSITORY_H

#include "EmailDao.h"
#include "EmailEntity.h"
#include "EmailGroupDao.h"
#include "EmailGroupEntity.h"
#include "GroupDao.h"
#include "GroupEntity.h"
#include "PageResult.h"
#include <cstdint>
#include <memory>

namespace addrbook {

class EmailRepository
{
  public:
    EmailRepository();
    ~EmailRepository() = default;

    uint32_t CreateEmail(const EmailEntity& entity, const std::vector<uint32_t>& ids);
    bool DeleteAllEmails();
    bool IsMailExist(const std::vector<uint32_t>& email_ids) const;
    uint32_t GetEmailCount() const;
    bool CanAddGroup(const std::vector<uint32_t>& group_ids, uint32_t count_limit);
    uint32_t GetOrCreateEmail(const EmailEntity& entity);

    uint32_t AddGroup(const GroupEntity& entity, const std::vector<uint32_t>& ids);
    bool IsGroupExist(const std::vector<uint32_t>& group_ids) const;
    size_t GetGroupCount() const;
    bool CanAddEmail(const std::vector<uint32_t>& email_ids, uint32_t count_limit);
    uint32_t GetOrCreateGroup(const GroupEntity& entity, uint32_t email_rid);
    bool DeleteEmails(const std::vector<uint32_t>& email_ids);
    bool RemoveGroup(const std::vector<uint32_t>& group_ids);
    bool RemoveGroupByMailRid(uint32_t email_rid);

    bool UpdateEmail(const EmailEntity& entity, const std::vector<uint32_t>& new_group_rids);
    bool UpdateGroup(const std::shared_ptr<GroupEntity>& entity_sptr);

    PageResult GetEmailsByKeyword(const PageQueryParam& query_param);

  private:
    EmailDao m_mail_dao;
    GroupDao m_group_dao;
    EmailGroupDao m_mail_group_dao;
};
}  // namespace addrbook

#endif  // EMAILREPOSITORY_H