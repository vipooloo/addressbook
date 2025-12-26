#ifndef EMAILREPOSITORY_H
#define EMAILREPOSITORY_H

#include "AddressResultCodeDefs.h"
#include "EmailEntity.h"
#include "GroupEntity.h"
#include "MailGroupRelation.h"
#include <cstdint>
#include <memory>

class EmailDao;
class GroupDao;
class MailGroupDao;

class EmailRepository
{
  public:
    EmailRepository();
    ~EmailRepository() = default;

    uint32_t AddEmail(const std::shared_ptr<EmailEntity>& entity_sptr, const std::vector<uint32_t>& rids);
    bool IsMailExist(const std::vector<uint32_t>& mail_ids) const;
    uint32_t GetEmailCount() const;
    bool CanAddGroup(const std::vector<uint32_t>& mail_ids, uint32_t count_limit);

    uint32_t AddGroup(const std::shared_ptr<GroupEntity>& entity_sptr, const std::vector<uint32_t>& rids);
    bool IsGroupExist(const std::vector<uint32_t>& group_ids) const;
    size_t GetGroupCount() const;
    bool CanAddEmail(const std::vector<uint32_t>& mail_ids, uint32_t count_limit);

    bool RemoveEmail(const std::vector<uint32_t>& mail_ids);
    bool RemoveGroup(const std::vector<uint32_t>& group_ids);

    bool UpdateEmail(const std::shared_ptr<EmailEntity>& entity_sptr);
    bool UpdateGroup(const std::shared_ptr<GroupEntity>& entity_sptr);

    std::pair<std::vector<uint32_t>, std::vector<uint32_t>> GetChangedGroup(const std::vector<uint32_t>& group_rids, uint32_t mail_rid);

  private:
    bool
    AddEmailToGroupRelation(const std::vector<std::shared_ptr<AbstractEntity>>& items);

  private:
    std::shared_ptr<EmailDao> m_mail_dao_sptr;
    std::shared_ptr<GroupDao> m_group_dao_sptr;
    std::shared_ptr<MailGroupDao> m_mail_group_dao_sptr;
};

#endif  // EMAILREPOSITORY_H