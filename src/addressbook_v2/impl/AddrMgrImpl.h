#ifndef ADDRMGRIMPL_H
#define ADDRMGRIMPL_H

#include "AddressMgrDefs.h"
#include "EmailService.h"
#include "EventDispatcher.h"
#include "EventLoop.h"
#include <mutex>

namespace addrbook {
class AddrMgrImpl
{
  public:
    static AddrMgrImpl& GetInstance()
    {
        static AddrMgrImpl instance;
        return instance;
    }

    std::pair<ResultCode, EmailDto> AddEmail(const EmailDto& dto);
    ResultCode RemoveEmail(const std::vector<uint32_t>& rids);
    void ClearAllEmails();
    ResultCode UpdateEmail(const EmailDto& dto);
    std::pair<ResultCode, SearchEmailResult> SearchEmail(const std::string& keyword, uint32_t current_page, uint32_t page_size);
    ResultCode ImportEmails(const std::string& file_path, const ImportExportCallback& cb);
    ResultCode ExportEmails(const std::string& file_path, const ImportExportCallback& cb);
    /*---------------------------------------------------*/
    std::pair<ResultCode, GroupDto> AddGroup(const GroupDto& dto);
    ResultCode RemoveGroup(const std::vector<uint32_t>& rids);
    ResultCode UpdateGroup(const GroupDto& dto);

    void Register(const std::shared_ptr<IAddressMgrDataObserver>& observer);
    void Unregister(const std::shared_ptr<IAddressMgrDataObserver>& observer);

  private:
    AddrMgrImpl();
    ~AddrMgrImpl();
    AddrMgrImpl(const AddrMgrImpl&) = delete;
    AddrMgrImpl& operator=(const AddrMgrImpl&) = delete;
    AddrMgrImpl(AddrMgrImpl&&) noexcept = delete;
    AddrMgrImpl& operator=(AddrMgrImpl&&) noexcept = delete;

    void EventHandler(const std::shared_ptr<IEvent>& evt_sptr);
    void ImportEmailsSync(const std::string& file_path, const ImportExportCallback& cb);
    void ExportEmailsSync(const std::string& file_path, const ImportExportCallback& cb);

  private:
    EventLoop m_evt_loop;
    std::mutex m_mtx;
    EventDispatcher m_evt_dispatcher;
    EmailService m_email_srv;
};
}  // namespace addrbook

#endif  // ADDRMGRIMPL_H