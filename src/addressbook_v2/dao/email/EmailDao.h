#ifndef EMAILDAO_H
#define EMAILDAO_H

#include "AbstractDao.h"

namespace addrbook {
class EmailDao : public AbstractDao
{
  public:
    EmailDao();
    ~EmailDao() override = default;
    bool Create() override;
    bool Insert(const std::shared_ptr<AbstractEntity>& in_entity_sptr, const std::shared_ptr<AbstractEntity>& out_entity_sptr) override;
    bool Update(const std::shared_ptr<AbstractEntity>& entity_sptr) override;
    PageResult FindAll(uint32_t page_num, uint32_t page_size) override;
    PageResult FindByPage(const std::string& keyword, uint32_t page_num, uint32_t page_size) override;

  protected:
    std::shared_ptr<AbstractEntity> OnCreateEntity(const SQLite::Statement& stmt) override;

  private:
};
}  // namespace addrbook

#endif  // EMAILDAO_H