#ifndef ADDRESSMANAGER_H
#define ADDRESSMANAGER_H

#include "AddressMgrInc.h"

class AddressManager
{
  public:
    static std::pair<ResultCode, EmailDto> AddEmail(const EmailDto& dto);
    static ResultCode RemoveEmail(const std::vector<uint32_t>& rids);
    static ResultCode UpdateEmail(const EmailDto& dto);
    static void ClearAllEmails();
    static std::pair<ResultCode, SearchEmailResult> SearchEmail(const std::string& keyword, uint32_t current_page, uint32_t page_size);
    static ResultCode ImportEmails(const std::string& file_path, const ImportExportCallback& cb);
    static ResultCode ExportEmails(const ImportExportCallback& cb);
    static ResultCode ExportEmails(const std::string& file_path, const ImportExportCallback& cb);
    /*---------------------------------------------------*/
    static std::pair<ResultCode, GroupDto> AddGroup(const GroupDto& dto);
    static ResultCode RemoveGroup(const std::vector<uint32_t>& rids);
    static ResultCode UpdateGroup(const GroupDto& dto);
    /*---------------------------------------------------*/
    void Register(const std::shared_ptr<IAddressMgrDataObserver>& observer);
    void Unregister(const std::shared_ptr<IAddressMgrDataObserver>& observer);

  private:
    AddressManager() = delete;
    ~AddressManager() = delete;
    AddressManager(const AddressManager&) = delete;
    AddressManager& operator=(const AddressManager&) = delete;
    AddressManager(AddressManager&&) noexcept = delete;
    AddressManager& operator=(AddressManager&&) noexcept = delete;
};

#endif  // ADDRESSMANAGER_H