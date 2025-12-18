#ifndef GROUPDAO_H
#define GROUPDAO_H

#include "AbstractDao.h"

class GroupDao : public AbstractDao
{
  public:
    GroupDao();
    ~GroupDao() override = default;
    bool Init() override;
    size_t GetCount() const override;
    bool Insert(const std::shared_ptr<AbstractEntity>& in_entity_sptr, const std::shared_ptr<AbstractEntity>& out_entity_sptr) override;
    bool IsExist(const std::vector<uint32_t>& rids);
};

#endif  // GROUPDAO_H