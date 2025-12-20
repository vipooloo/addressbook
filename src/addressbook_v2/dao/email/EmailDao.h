#ifndef EMAILDAO_H
#define EMAILDAO_H

#include "AbstractDao.h"

class EmailDao : public AbstractDao
{
  public:
    EmailDao();
    ~EmailDao() override = default;
    bool Init() override;
    size_t GetCount() const override;
    bool Insert(const std::shared_ptr<AbstractEntity>& in_entity_sptr, const std::shared_ptr<AbstractEntity>& out_entity_sptr) override;
    bool IsExist(const std::vector<uint32_t>& rids) override;

    bool Remove(const std::vector<uint32_t>& rids) override;
    bool RemoveAll() override;
};

#endif  // EMAILDAO_H