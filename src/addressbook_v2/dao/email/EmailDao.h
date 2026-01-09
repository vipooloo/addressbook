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
    bool Update(const std::shared_ptr<EmailEntity>& entity_sptr);
    PageResult FindAll(uint32_t page_num, uint32_t page_size);
    PageResult FindByPage(const std::string& keyword, uint32_t page_num, uint32_t page_size);

  protected:
    std::shared_ptr<AbstractEntity> OnCreateEntity(const SQLite::Statement& stmt) override;

  private:
};
}  // namespace addrbook

#endif  // EMAILDAO_H