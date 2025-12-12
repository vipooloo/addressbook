#ifndef GroupRepository_H
#define GroupRepository_H

#include "AddressResultCodeDefs.h"
#include "GroupEntity.h"
#include <memory>
#include <stdint.h>

class AbstractDao;

class GroupRepository
{
  public:
    GroupRepository();
    ~GroupRepository() = default;
    ErrorCode AddGroup(const GroupEntity& entity);
    bool IsGroupExist(const std::vector<uint32_t>& group_ids) const;
    size_t GetGroupCount() const;

  private:
    std::shared_ptr<AbstractDao> m_group_dao_sptr;
};

#endif  // GroupRepository_H