#ifndef ADDRCENTER_H
#define ADDRCENTER_H

#include "AddrCenterDefs.h"
#include "AddrCenterSearchResult.h"
#include "EmailDto.h"
#include "GroupDto.h"
#include "IAddrCenterDataObserver.h"
#include <memory>

class AddrCenter
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
    void Register(const std::shared_ptr<IAddrCenterDataObserver>& observer);
    void Unregister(const std::shared_ptr<IAddrCenterDataObserver>& observer);

  private:
    AddrCenter() = delete;
    ~AddrCenter() = delete;
    AddrCenter(const AddrCenter&) = delete;
    AddrCenter& operator=(const AddrCenter&) = delete;
    AddrCenter(AddrCenter&&) = delete;
    AddrCenter& operator=(AddrCenter&&) = delete;
};

#endif  // ADDRCENTER_H