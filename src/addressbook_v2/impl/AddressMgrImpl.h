#ifndef ADDRESSMGRIMPL_H
#define ADDRESSMGRIMPL_H

#include "AddressManagerDefs.h"
#include "AddressMgrEvtDispatcher.h"
#include "EmailService.h"
#include "EventLoop.h"
#include <mutex>

class AddressMgrImpl
{
  public:
    static AddressMgrImpl& GetInstance()
    {
        static AddressMgrImpl instance;
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

    void Register(const std::shared_ptr<IAddressDataObserver>& observer);
    void Unregister(const std::shared_ptr<IAddressDataObserver>& observer);

  private:
    AddressMgrImpl();
    ~AddressMgrImpl();
    AddressMgrImpl(const AddressMgrImpl&) = delete;
    AddressMgrImpl& operator=(const AddressMgrImpl&) = delete;
    AddressMgrImpl(AddressMgrImpl&&) = delete;
    AddressMgrImpl& operator=(AddressMgrImpl&&) = delete;

    void EventHandler(const std::shared_ptr<IEvent>& evt_sptr);

  private:
    EmailService m_email_srv;
    EventLoop m_evt_loop;
    std::mutex m_mtx;
    AddressMgrEvtDispatcher m_evt_dispatcher;
};

#endif  // ADDRESSMGRIMPL_H