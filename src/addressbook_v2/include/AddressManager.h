#ifndef ADDRESSMANAGER_H
#define ADDRESSMANAGER_H

#include "AddressMgrInc.h"

class AddressManager
{
  public:
    static std::pair<ResultCode, EmailDto> CreateEmail(const EmailDto& dto);
    static ResultCode DeleteEmails(const std::vector<uint32_t>& ids);
    static ResultCode DeleteAllEmails();
    static ResultCode UpdateEmail(const EmailDto& dto);
    static std::pair<ResultCode, EmailPageResult> PageQueryEmail(const PageQueryParam& query_param);
    static ResultCode ImportEmails(const std::string& file_path, const ImportExportCallback& cb);
    static ResultCode ExportEmails(const ImportExportCallback& cb);
    /*---------------------------------------------------*/
    static std::pair<ResultCode, GroupDto> CreateGroup(const GroupDto& dto);
    static ResultCode DeleteGroups(const std::vector<uint32_t>& ids);
    static ResultCode DeleteAllGroups();
    static ResultCode DeleteGroup(const std::vector<uint32_t>& ids);
    static ResultCode UpdateGroup(const GroupDto& dto);
    static std::pair<ResultCode, GroupPageResult> PageQueryGroup(const PageQueryParam& query_param);
    /*---------------------------------------------------*/
    static ResultCode Register(const std::shared_ptr<IAddressMgrDataObserver>& observer);
    static ResultCode Unregister(const std::shared_ptr<IAddressMgrDataObserver>& observer);

  private:
    AddressManager() = delete;
    ~AddressManager() = delete;
    AddressManager(const AddressManager&) = delete;
    AddressManager& operator=(const AddressManager&) = delete;
    AddressManager(AddressManager&&) noexcept = delete;
    AddressManager& operator=(AddressManager&&) noexcept = delete;
};

#endif  // ADDRESSMANAGER_H