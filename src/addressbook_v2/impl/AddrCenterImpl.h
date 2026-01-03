#ifndef ADDRCENTERIMPL_H
#define ADDRCENTERIMPL_H

#include "EmailService.h"
#include "EventLoop.h"

class AddrCenterImpl
{
  public:
    static AddrCenterImpl& GetInstance()
    {
        static AddrCenterImpl instance;
        return instance;
    }

    std::pair<ErrorCode, EmailDto> AddEmail(const EmailDto& dto);
    std::pair<ErrorCode, GroupDto> AddGroup(const GroupDto& dto);
    ErrorCode RemoveEmail(const std::vector<uint32_t>& rids);
    ErrorCode RemoveGroup(const std::vector<uint32_t>& rids);
    ErrorCode UpdateEmail(const EmailDto& dto);
    ErrorCode UpdateGroup(const GroupDto& dto);
    std::pair<ErrorCode, SearchResult> SearchEmail(const std::string& keyword, uint32_t current_page, uint32_t page_size);

  private:
    AddrCenterImpl();
    ~AddrCenterImpl() = default;
    AddrCenterImpl(const AddrCenterImpl&) = delete;
    AddrCenterImpl& operator=(const AddrCenterImpl&) = delete;
    AddrCenterImpl(AddrCenterImpl&&) = delete;
    AddrCenterImpl& operator=(AddrCenterImpl&&) = delete;

    void EventHandler(const std::shared_ptr<IEvent>& evt_sptr);

  private:
    EmailService m_email_srv;
    EventLoop m_evt_loop;
};

#endif  // ADDRCENTERIMPL_H