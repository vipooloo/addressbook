#ifndef EMAILSERVICE_H
#define EMAILSERVICE_H

#include "AddressResultCodeDefs.h"
#include "EmailDto.h"
#include "GroupDto.h"
#include <memory>
#include <vector>

class EmailRepository;

class EmailService
{
  public:
    EmailService();
    ~EmailService() = default;

    std::pair<ErrorCode, EmailDto> AddEmail(const EmailDto& dto);
    std::pair<ErrorCode, GroupDto> AddGroup(const GroupDto& dto);

    ErrorCode RemoveEmail(const std::vector<uint32_t>& rids);

  private:
    std::shared_ptr<EmailRepository> m_mail_rep_sptr;
};

#endif  // EMAILSERVICE_H