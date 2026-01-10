#ifndef MAILGROUPDAO_H
#define MAILGROUPDAO_H

#include "AbstractDao.h"
#include "MailGroupRelation.h"

namespace addrbook {
class MailGroupDao : public AbstractDao
{
  public:
    MailGroupDao();
    ~MailGroupDao() override = default;
    bool Create() override;
    bool InsertBatch(const std::vector<MailGroupRelation>& items);
    bool HasMemberOverGroupLimit(const std::vector<uint32_t>& group_ids, uint32_t limit) const;
    bool HasMemberOverEMailLimit(const std::vector<uint32_t>& group_ids, uint32_t limit) const;

    bool RemoveByEmailRid(uint32_t email_rid);

  private:
    bool HasMemberOverLimit(const std::vector<uint32_t>& ids, const std::string& content, uint32_t limit) const;
};
}  // namespace addrbook

#endif  // MAILGROUPDAO_H