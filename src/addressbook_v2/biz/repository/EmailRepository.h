#ifndef EMAILREPOSITORY_H
#define EMAILREPOSITORY_H

#include "AddressResultCodeDefs.h"
#include "EmailDao.h"
#include <memory>
#include <stdint.h>

class EmailRepository
{
  public:
    EmailRepository();
    ~EmailRepository() = default;
    void SetEmailDao(const std::shared_ptr<EmailDao>& email_dao_sptr)
    {
        m_mail_dao_sptr = email_dao_sptr;
        if (m_mail_dao_sptr)
        {
            m_mail_dao_sptr->Init();
        }
    }
    ErrorCode AddEmail(const EmailEntity& entity);
    uint32_t GetEmailCount() const;

  private:
    std::shared_ptr<EmailDao> m_mail_dao_sptr;
};

#endif  // EMAILREPOSITORY_H