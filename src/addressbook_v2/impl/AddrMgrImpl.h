#ifndef ADDRMGRIMPL_H
#define ADDRMGRIMPL_H

#include "AddressMgrDefs.h"
#include "AddressMgrInc.h"
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

    std::pair<ResultCode, EmailDto> CreateEmail(const EmailDto& dto);
    ResultCode DeleteEmails(const std::vector<uint32_t>& ids);
    ResultCode DeleteAllEmails();
    ResultCode UpdateEmail(const EmailDto& dto);
    std::pair<ResultCode, EmailPageResult> PageQueryEmail(const PageQueryParam& query_param);
    ResultCode ImportEmails(const std::string& file_path, const ImportExportCallback& cb);
    ResultCode ExportEmails(const std::string& file_path, const ImportExportCallback& cb);
    /*---------------------------------------------------*/
    std::pair<ResultCode, GroupDto> CreateGroup(const GroupDto& dto);
    ResultCode DeleteGroups(const std::vector<uint32_t>& ids);
    ResultCode DeleteAllGroups();
    ResultCode UpdateGroup(const GroupDto& dto);
    std::pair<ResultCode, GroupPageResult> PageQueryGroup(const PageQueryParam& query_param);
    /*---------------------------------------------------*/
    ResultCode Register(const std::shared_ptr<IAddressMgrDataObserver>& observer);
    ResultCode Unregister(const std::shared_ptr<IAddressMgrDataObserver>& observer);

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