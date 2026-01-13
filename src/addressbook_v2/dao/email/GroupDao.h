#ifndef GROUPDAO_H
#define GROUPDAO_H

#include "AbstractDao.h"
#include "GroupEntity.h"

namespace addrbook {
class GroupDao : public AbstractDao
{
  public:
    GroupDao();
    ~GroupDao() override = default;
    bool Create() override;
    std::pair<bool, GroupEntity> Insert(const GroupEntity& entity);
    uint32_t GetGroupRid(const GroupEntity& entity);
};
}  // namespace addrbook

#endif  // GROUPDAO_H