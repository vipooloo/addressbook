#ifndef EMAILREPOSITORY_H
#define EMAILREPOSITORY_H

#include "AddressResultCodeDefs.h"
#include "EmailEntity.h"
#include "GroupEntity.h"
#include "MailGroupRelation.h"
#include <memory>
#include <stdint.h>

class AbstractDao;

class EmailRepository
{
  public:
    EmailRepository();
    ~EmailRepository() = default;
    bool AddEmail(const EmailEntity& entity, std::shared_ptr<AbstractEntity>& out_entity_sptr);
    bool IsMailExist(const std::vector<uint32_t>& mail_ids) const;
    uint32_t GetEmailCount() const;
    bool CanAddGroup(const std::vector<uint32_t>& mail_ids, uint32_t add_count);

    bool AddGroup(const GroupEntity& entity, std::shared_ptr<AbstractEntity>& out_entity_sptr);
    bool IsGroupExist(const std::vector<uint32_t>& group_ids) const;
    size_t GetGroupCount() const;
    bool CanAddEmail(const std::vector<uint32_t>& group_ids, uint32_t add_count);

    bool AddEmailToGroupRelation(const MailGroupRelation& entity);

  private:
    std::shared_ptr<AbstractDao> m_mail_dao_sptr;
    std::shared_ptr<AbstractDao> m_group_dao_sptr;
    std::shared_ptr<AbstractDao> m_mail_group_dao_sptr;
};

#endif  // EMAILREPOSITORY_H