#ifndef EMAILSERVICE_H
#define EMAILSERVICE_H

#include "AddrCenterResCodeDefs.h"
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

    std::pair<ResultCode, EmailDto> AddEmail(const EmailDto& dto);
    std::pair<ResultCode, GroupDto> AddGroup(const GroupDto& dto);

    ResultCode RemoveEmail(const std::vector<uint32_t>& rids);
    ResultCode RemoveGroup(const std::vector<uint32_t>& rids);

    ResultCode UpdateEmail(const EmailDto& dto);
    ResultCode UpdateGroup(const GroupDto& dto);

    std::pair<ResultCode, SearchResult> SearchEmail(const std::string& keyword, uint32_t current_page, uint32_t page_size);

  private:
    std::shared_ptr<EmailRepository> m_mail_rep_sptr;
};

#endif  // EMAILSERVICE_H