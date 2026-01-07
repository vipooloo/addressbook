#ifndef GROUPDAO_H
#define GROUPDAO_H

#include "AbstractDao.h"

namespace addrbook {
class GroupDao : public AbstractDao
{
  public:
    GroupDao();
    ~GroupDao() override = default;
    bool Create() override;
    bool Insert(const std::shared_ptr<AbstractEntity>& in_entity_sptr, const std::shared_ptr<AbstractEntity>& out_entity_sptr) override;
};
}  // namespace addrbook

#endif  // GROUPDAO_H