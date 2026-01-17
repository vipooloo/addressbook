#ifndef EMAILDAO_H
#define EMAILDAO_H

#include "AbstractDao.h"
#include "EmailEntity.h"

namespace addrbook {
class EmailDao : public AbstractDao
{
  public:
    EmailDao();
    ~EmailDao() override = default;
    bool Create() override;
    std::pair<bool, EmailEntity> Insert(const EmailEntity& entity);
    uint32_t GetEmailRid(const EmailEntity& entity);
    bool Update(const EmailEntity& entity);
    PageResult FindAll(const QueryParam& query_param);
    PageResult FindByPage(const QueryParam& query_param);

  protected:
    std::shared_ptr<AbstractEntity> OnCreateEntity(const SQLite::Statement& stmt) override;

  private:
};
}  // namespace addrbook

#endif  // EMAILDAO_H