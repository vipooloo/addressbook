#ifndef ADDRCENTER_H
#define ADDRCENTER_H

#include "AddrCenterDefs.h"
#include "AddressResultCodeDefs.h"
#include "EmailDto.h"
#include "GroupDto.h"
#include "SearchResult.h"

class AddrCenter
{
  public:
    static std::pair<ErrorCode, EmailDto> AddEmail(const EmailDto& dto);
    static std::pair<ErrorCode, GroupDto> AddGroup(const GroupDto& dto);
    static ErrorCode RemoveEmail(const std::vector<uint32_t>& rids);
    static ErrorCode RemoveGroup(const std::vector<uint32_t>& rids);
    static ErrorCode UpdateEmail(const EmailDto& dto);
    static ErrorCode UpdateGroup(const GroupDto& dto);
    static std::pair<ErrorCode, SearchResult> SearchEmail(const std::string& keyword, uint32_t current_page , uint32_t page_size);
    /*---------------------------------------------------*/
    static ErrorCode ImportEmails(const std::string& file_path, const ImportExportCallback& cb);
    static ErrorCode ExportEmails(const ImportExportCallback& cb);
    static ErrorCode ExportEmails(const std::string& file_path, const ImportExportCallback& cb);

  private:
    AddrCenter() = delete;
    ~AddrCenter() = delete;
    AddrCenter(const AddrCenter&) = delete;
    AddrCenter& operator=(const AddrCenter&) = delete;
    AddrCenter(AddrCenter&&) = delete;
    AddrCenter& operator=(AddrCenter&&) = delete;
};

#endif  // ADDRCENTER_H