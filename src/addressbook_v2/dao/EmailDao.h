#ifndef EMAILDAO_H
#define EMAILDAO_H

#include "AbstractDao.h"
#include "EmailEntity.h"

class EmailDao : public AbstractDao
{
  public:
    explicit EmailDao(const std::shared_ptr<SQLiteConn>& db_sptr);
    bool Init() override;
    DaoErrCode Insert(const AbstractEntity& entity) override;
};

#endif  // EMAILDAO_H