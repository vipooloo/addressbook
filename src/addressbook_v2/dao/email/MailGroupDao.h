#ifndef MAILGROUPDAO_H
#define MAILGROUPDAO_H

#include "AbstractDao.h"
#include <map>

class MailGroupDao : public AbstractDao
{
  public:
    MailGroupDao();
    ~MailGroupDao() override = default;
    bool Init() override;
    bool Insert(const std::shared_ptr<AbstractEntity>& in_entity_sptr, const std::shared_ptr<AbstractEntity>& out_entity_sptr) override;
    bool InsertBatch(const std::vector<std::shared_ptr<AbstractEntity>>& items) override;
    bool HasMemberOverGroupLimit(const std::vector<uint32_t>& group_ids, uint32_t limit) const;
    bool HasMemberOverEMailLimit(const std::vector<uint32_t>& group_ids, uint32_t limit) const;

    bool RemoveByEmailRid(uint32_t email_rid);

  private:
    bool HasMemberOverLimit(const std::vector<uint32_t>& ids, const std::string& content, uint32_t limit) const;
};

#endif  // MAILGROUPDAO_H