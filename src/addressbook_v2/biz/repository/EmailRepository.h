#ifndef EMAILREPOSITORY_H
#define EMAILREPOSITORY_H

#include "AddressResultCodeDefs.h"
#include "EmailEntity.h"
#include <memory>
#include <stdint.h>

class AbstractDao;

class EmailRepository
{
  public:
    EmailRepository();
    ~EmailRepository() = default;
    ErrorCode AddEmail(const EmailEntity& entity);
    uint32_t GetEmailCount() const;

  private:
    std::shared_ptr<AbstractDao> m_mail_dao_sptr;
};

#endif  // EMAILREPOSITORY_H