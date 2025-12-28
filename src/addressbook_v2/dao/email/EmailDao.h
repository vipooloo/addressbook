#ifndef EMAILDAO_H
#define EMAILDAO_H

#include "AbstractDao.h"

class EmailDao : public AbstractDao
{
  public:
    EmailDao();
    ~EmailDao() override = default;
    bool Create() override;
    bool Insert(const std::shared_ptr<AbstractEntity>& in_entity_sptr, const std::shared_ptr<AbstractEntity>& out_entity_sptr) override;
    bool Update(const std::shared_ptr<AbstractEntity>& entity_sptr) override;
    std::pair<bool, PageResult> FindByPage(const QueryParams& params) override;
};

#endif  // EMAILDAO_H