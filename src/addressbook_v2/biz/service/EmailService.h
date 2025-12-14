#ifndef EMAILSERVICE_H
#define EMAILSERVICE_H

#include "AddressResultCodeDefs.h"
#include "EmailDto.h"
#include "EmailRepository.h"
#include "GroupDto.h"

class EmailService
{
  public:
    EmailService()
      : m_mail_rep{}
    {}
    ~EmailService() = default;

    std::pair<ErrorCode,EmailDto> AddEmail(const EmailDto& dto);
    ErrorCode AddGroup(const GroupDto& dto);

  private:
    EmailRepository m_mail_rep;
};

#endif  // EMAILSERVICE_H