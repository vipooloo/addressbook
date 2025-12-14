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
    bool Insert(const AbstractEntity& entity, std::shared_ptr<AbstractEntity>& out_entity_sptr) override;
};

#endif  // EMAILDAO_H