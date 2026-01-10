#ifndef EMAILGROUPDAO_H
#define EMAILGROUPDAO_H

#include "AbstractDao.h"
#include "EmailGroupEntity.h"

namespace addrbook {
class EmailGroupDao : public AbstractDao
{
  public:
    EmailGroupDao();
    ~EmailGroupDao() override = default;
    bool Create() override;
    bool InsertBatch(const std::vector<EmailGroupEntity>& items);
    bool HasMemberOverGroupLimit(const std::vector<uint32_t>& group_ids, uint32_t limit) const;
    bool HasMemberOverEMailLimit(const std::vector<uint32_t>& email_ids, uint32_t limit) const;
    bool RemoveByEmailRid(uint32_t email_rid);

  private:
    bool HasMemberOverLimit(const std::vector<uint32_t>& ids, const std::string& content, uint32_t limit) const;
};
}  // namespace addrbook

#endif  // EMAILGROUPDAO_H