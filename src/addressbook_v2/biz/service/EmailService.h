#ifndef EMAILSERVICE_H
#define EMAILSERVICE_H

#include "AddressMgrDefs.h"
#include "EmailDto.h"
#include "EmailRepository.h"
#include "EventDispatcher.h"
#include "GroupDto.h"
#include "SearchEmailResult.h"
#include <memory>
#include <vector>

namespace addrbook {

class EmailService
{
  public:
    explicit EmailService(EventDispatcher& dispatcher);
    ~EmailService() = default;

    std::pair<ResultCode, EmailDto> AddEmail(const EmailDto& dto);
    std::pair<ResultCode, GroupDto> AddGroup(const GroupDto& dto);
    ResultCode AddEmailAndGroup(const EmailDto& dto);

    ResultCode RemoveEmail(const std::vector<uint32_t>& rids);
    ResultCode RemoveGroup(const std::vector<uint32_t>& rids);

    ResultCode UpdateEmail(const EmailDto& dto);
    ResultCode UpdateGroup(const GroupDto& dto);

    std::pair<ResultCode, SearchEmailResult> SearchEmail(const std::string& keyword, uint32_t current_page, uint32_t page_size);

  private:
    void DataChanged(ResultCode res, ChangeType type);

  private:
    EventDispatcher& m_env_dispatcher;
    EmailRepository m_mail_rep;
};
}  // namespace addrbook

#endif  // EMAILSERVICE_H