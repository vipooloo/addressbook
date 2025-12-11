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
    DaoErrCode Insert(const AbstractEntity& entity) override;
};

#endif  // GROUPDAO_H