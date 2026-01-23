#ifndef EMAILSERVICE_H
#define EMAILSERVICE_H

#include "AddressMgrDefs.h"
#include "EmailDto.h"
#include "EmailPageResult.h"
#include "EmailRepository.h"
#include "EventDispatcher.h"
#include "GroupDto.h"
#include "GroupPageResult.h"
#include <memory>
#include <vector>

namespace addrbook {

class EmailService
{
  public:
    explicit EmailService(EventDispatcher& dispatcher);
    ~EmailService() = default;

    std::pair<ResultCode, EmailDto> CreateEmail(const EmailDto& dto);
    ResultCode DeleteAllEmails();

    std::pair<ResultCode, GroupDto> CreateGroup(const GroupDto& dto);
    ResultCode DeleteAllGroups();

    ResultCode AddEmailAndGroup(const EmailDto& dto);

    ResultCode DeleteEmails(const std::vector<uint32_t>& ids);
    ResultCode DeleteGroup(const std::vector<uint32_t>& ids);

    ResultCode UpdateEmail(const EmailDto& dto);
    ResultCode UpdateGroup(const GroupDto& dto);

    std::pair<ResultCode, EmailPageResult> PageQueryEmail(const PageQueryParam& query_param);
    std::pair<ResultCode, GroupPageResult> PageQueryGroup(const PageQueryParam& query_param);

  private:
    void DataChanged(ResultCode res, ChangeType type);

  private:
    EventDispatcher& m_env_dispatcher;
    EmailRepository m_mail_rep;
};
}  // namespace addrbook

#endif  // EMAILSERVICE_H