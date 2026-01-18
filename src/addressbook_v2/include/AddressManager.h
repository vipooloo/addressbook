#ifndef ADDRESSMANAGER_H
#define ADDRESSMANAGER_H

#include "AddressMgrInc.h"

class AddressManager
{
  public:
    static std::pair<ResultCode, EmailDto> CreateEmail(const EmailDto& dto);
    static ResultCode DeleteEmails(const std::vector<uint32_t>& ids);
    static ResultCode UpdateEmail(const EmailDto& dto);
    static ResultCode DeleteAllEmails();
    static std::pair<ResultCode, EmailPageResult> PageQueryEmail(const PageQueryParam& query_param);
    static ResultCode ImportEmails(const std::string& file_path, const ImportExportCallback& cb);
    static ResultCode ExportEmails(const ImportExportCallback& cb);
    /*---------------------------------------------------*/
    static std::pair<ResultCode, GroupDto> AddGroup(const GroupDto& dto);
    static ResultCode RemoveGroup(const std::vector<uint32_t>& ids);
    static ResultCode UpdateGroup(const GroupDto& dto);
    /*---------------------------------------------------*/
    static void Register(const std::shared_ptr<IAddressMgrDataObserver>& observer);
    static void Unregister(const std::shared_ptr<IAddressMgrDataObserver>& observer);

  private:
    AddressManager() = delete;
    ~AddressManager() = delete;
    AddressManager(const AddressManager&) = delete;
    AddressManager& operator=(const AddressManager&) = delete;
    AddressManager(AddressManager&&) noexcept = delete;
    AddressManager& operator=(AddressManager&&) noexcept = delete;
};

#endif  // ADDRESSMANAGER_H