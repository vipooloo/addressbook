#ifndef EMAILSERVICE_H
#define EMAILSERVICE_H

#include "AddressResultCodeDefs.h"
#include "EmailDto.h"
#include "EmailRepository.h"
#include "GroupDto.h"
#include "GroupRepository.h"

class EmailService
{
  public:
    EmailService()
      : m_mail_rep{}
      , m_group_repo{}
    {}
    ~EmailService() = default;

    ErrorCode AddEmail(const EmailDto& dto);
    ErrorCode AddGroup(const GroupDto& dto);

  private:
    EmailRepository m_mail_rep;
    GroupRepository m_group_repo;
};

#endif  // EMAILSERVICE_H