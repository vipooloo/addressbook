#ifndef MAILGROUPDAO_H
#define MAILGROUPDAO_H

#include "AbstractDao.h"

class MailGroupDao : public AbstractDao
{
  public:
    MailGroupDao();
    ~MailGroupDao() override = default;
    bool Init() override;
    size_t GetCount() const override;
    size_t CountByCond(const MailGroupMappingQueryCond& cond) const override;
};

#endif  // MAILGROUPDAO_H