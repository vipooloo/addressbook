#ifndef ADDRCENTERIMPL_H
#define ADDRCENTERIMPL_H

#include "AddrCenterDefs.h"
#include "AddrEventDispatcher.h"
#include "EmailService.h"
#include "EventLoop.h"
#include <mutex>

class AddrCenterImpl
{
  public:
    static AddrCenterImpl& GetInstance()
    {
        static AddrCenterImpl instance;
        return instance;
    }

    std::pair<ResultCode, EmailDto> AddEmail(const EmailDto& dto);
    std::pair<ResultCode, GroupDto> AddGroup(const GroupDto& dto);
    ResultCode RemoveEmail(const std::vector<uint32_t>& rids);
    ResultCode RemoveGroup(const std::vector<uint32_t>& rids);
    ResultCode UpdateEmail(const EmailDto& dto);
    ResultCode UpdateGroup(const GroupDto& dto);
    std::pair<ResultCode, AddrCenterSearchResult> SearchEmail(const std::string& keyword, uint32_t current_page, uint32_t page_size);

    ResultCode ImportEmails(const std::string& file_path, const ImportExportCallback& cb);
    ResultCode ExportEmails(const std::string& file_path, const ImportExportCallback& cb);

    void Register(const std::shared_ptr<IAddrCenterDataObserver>& observer);
    void Unregister(const std::shared_ptr<IAddrCenterDataObserver>& observer);

  private:
    AddrCenterImpl();
    ~AddrCenterImpl();
    AddrCenterImpl(const AddrCenterImpl&) = delete;
    AddrCenterImpl& operator=(const AddrCenterImpl&) = delete;
    AddrCenterImpl(AddrCenterImpl&&) = delete;
    AddrCenterImpl& operator=(AddrCenterImpl&&) = delete;

    void EventHandler(const std::shared_ptr<IEvent>& evt_sptr);

  private:
    EmailService m_email_srv;
    EventLoop m_evt_loop;
    std::mutex m_mtx;
    AddrEventDispatcher m_evt_dispatcher;
};

#endif  // ADDRCENTERIMPL_H