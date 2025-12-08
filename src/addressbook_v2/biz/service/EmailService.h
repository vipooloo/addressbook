#ifndef EMAILSERVICE_H
#define EMAILSERVICE_H

#include "AddressResultCodeDefs.h"
#include "EmailDto.h"

class EmailRepository;

class EmailService
{
  public:
    explicit EmailService(const std::shared_ptr<EmailRepository>& repo_sptr)
      : m_repo_sptr{repo_sptr}
    {}
    ~EmailService() = default;

    ErrorCode AddEmail(const EmailDto& dto);

  private:
    std::shared_ptr<EmailRepository> m_repo_sptr;
};

#endif  // EMAILSERVICE_H