#ifndef EMAILSERVICE_H
#define EMAILSERVICE_H

#include "AddressResultCodeDefs.h"
#include "EmailDto.h"
#include "GroupDto.h"
#include "SearchResult.h"
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
    ErrorCode RemoveGroup(const std::vector<uint32_t>& rids);

    ErrorCode UpdateEmail(const EmailDto& dto);
    ErrorCode UpdateGroup(const GroupDto& dto);

    std::pair<ErrorCode, SearchResult> SearchEmail(const std::string& keyword, uint32_t current_page, uint32_t page_size);

  private:
    std::shared_ptr<EmailRepository> m_mail_rep_sptr;
};

#endif  // EMAILSERVICE_H