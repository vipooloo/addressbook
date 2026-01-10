#ifndef EMAILREPOSITORY_H
#define EMAILREPOSITORY_H

#include "EmailDao.h"
#include "EmailEntity.h"
#include "GroupDao.h"
#include "GroupEntity.h"
#include "MailGroupDao.h"
#include "MailGroupRelation.h"
#include "PageResult.h"
#include <cstdint>
#include <memory>

namespace addrbook {

class EmailRepository
{
  public:
    EmailRepository();
    ~EmailRepository() = default;

    uint32_t AddEmail(const EmailEntity& entity, const std::vector<uint32_t>& rids);
    bool IsMailExist(const std::vector<uint32_t>& mail_ids) const;
    uint32_t GetEmailCount() const;
    bool CanAddGroup(const std::vector<uint32_t>& group_ids, uint32_t count_limit);
    std::vector<uint32_t> GetGroupRidsByMailRid(uint32_t mail_rid);

    uint32_t AddGroup(const GroupEntity& entity, const std::vector<uint32_t>& rids);
    bool IsGroupExist(const std::vector<uint32_t>& group_ids) const;
    size_t GetGroupCount() const;
    bool CanAddEmail(const std::vector<uint32_t>& mail_ids, uint32_t count_limit);

    bool RemoveEmail(const std::vector<uint32_t>& mail_ids);
    bool RemoveGroup(const std::vector<uint32_t>& group_ids);
    bool RemoveGroupByMailRid(uint32_t mail_rid);

    bool UpdateEmail(const EmailEntity& entity, const std::vector<uint32_t>& new_group_rids);
    bool UpdateGroup(const std::shared_ptr<GroupEntity>& entity_sptr);

    PageResult GetEmailsByKeyword(const std::string& keyword, uint32_t page_num, uint32_t page_size);

  private:
    bool AddEmailToGroupRelation(const std::vector<MailGroupRelation>& items);

  private:
    EmailDao m_mail_dao;
    GroupDao m_group_dao;
    MailGroupDao m_mail_group_dao;
};
}  // namespace addrbook

#endif  // EMAILREPOSITORY_H