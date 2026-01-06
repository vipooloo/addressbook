#ifndef ADDRESSMANAGER_H
#define ADDRESSMANAGER_H

#include "AddrCenterSearchResult.h"
#include "AddressManagerDefs.h"
#include "EmailDto.h"
#include "GroupDto.h"
#include "IAddressDataObserver.h"
#include <memory>

class AddressManager
{
  public:
    static std::pair<ResultCode, EmailDto> AddEmail(const EmailDto& dto);
    static std::pair<ResultCode, GroupDto> AddGroup(const GroupDto& dto);
    static ResultCode RemoveEmail(const std::vector<uint32_t>& rids);
    static ResultCode RemoveGroup(const std::vector<uint32_t>& rids);
    static ResultCode UpdateEmail(const EmailDto& dto);
    static ResultCode UpdateGroup(const GroupDto& dto);
    static std::pair<ResultCode, AddrCenterSearchResult> SearchEmail(const std::string& keyword, uint32_t current_page, uint32_t page_size);

    /*---------------------------------------------------*/
    static ResultCode ImportEmails(const std::string& file_path, const ImportExportCallback& cb);
    static ResultCode ExportEmails(const ImportExportCallback& cb);
    static ResultCode ExportEmails(const std::string& file_path, const ImportExportCallback& cb);

    /*---------------------------------------------------*/
    void Register(const std::shared_ptr<IAddressDataObserver>& observer);
    void Unregister(const std::shared_ptr<IAddressDataObserver>& observer);

  private:
    AddressManager() = delete;
    ~AddressManager() = delete;
    AddressManager(const AddressManager&) = delete;
    AddressManager& operator=(const AddressManager&) = delete;
    AddressManager(AddressManager&&) = delete;
    AddressManager& operator=(AddressManager&&) = delete;
};

#endif  // ADDRESSMANAGER_H