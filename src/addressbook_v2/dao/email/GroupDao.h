#ifndef GROUPDAO_H
#define GROUPDAO_H

#include "AbstractDao.h"

class GroupDao : public AbstractDao
{
  public:
    GroupDao();
    ~GroupDao() override = default;
    bool Init() override;
    bool Insert(const std::shared_ptr<AbstractEntity>& in_entity_sptr, const std::shared_ptr<AbstractEntity>& out_entity_sptr) override;
};

#endif  // GROUPDAO_H