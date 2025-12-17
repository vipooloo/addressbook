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
    bool Insert(const std::shared_ptr<AbstractEntity>& in_entity_sptr, const std::shared_ptr<AbstractEntity>& out_entity_sptr) override;
    bool InsertBatch(const std::vector<std::shared_ptr<AbstractEntity>>& items) override;
};

#endif  // MAILGROUPDAO_H