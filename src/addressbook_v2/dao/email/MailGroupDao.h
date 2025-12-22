#ifndef MAILGROUPDAO_H
#define MAILGROUPDAO_H

#include "AbstractDao.h"
#include <map>

class MailGroupDao : public AbstractDao
{
  public:
    MailGroupDao();
    ~MailGroupDao() override = default;
    bool Init() override;
    bool Insert(const std::shared_ptr<AbstractEntity>& in_entity_sptr, const std::shared_ptr<AbstractEntity>& out_entity_sptr) override;
    bool InsertBatch(const std::vector<std::shared_ptr<AbstractEntity>>& items) override;
    size_t CountByCond(const MailGroupMappingQueryCond& cond) const override;
    std::map<uint32_t, uint32_t> GetGroupEmailCounts(const std::vector<uint32_t>& rids, bool is_mail) const;
};

#endif  // MAILGROUPDAO_H