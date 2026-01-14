#include "AddrMgrImpl.h"
#include "AddrMgrUtilities.h"
#include "AddressManager.h"

std::pair<ResultCode, EmailDto> AddressManager::AddEmail(const EmailDto& dto)
{
    return addrbook::AddrMgrImpl::GetInstance().AddEmail(dto);
}

std::pair<ResultCode, GroupDto> AddressManager::AddGroup(const GroupDto& dto)
{
    return addrbook::AddrMgrImpl::GetInstance().AddGroup(dto);
}

ResultCode AddressManager::RemoveEmail(const std::vector<uint32_t>& rids)
{
    return addrbook::AddrMgrImpl::GetInstance().RemoveEmail(rids);
}

ResultCode AddressManager::RemoveGroup(const std::vector<uint32_t>& rids)
{
    return addrbook::AddrMgrImpl::GetInstance().RemoveGroup(rids);
}

ResultCode AddressManager::UpdateEmail(const EmailDto& dto)
{
    return addrbook::AddrMgrImpl::GetInstance().UpdateEmail(dto);
}

void AddressManager::ClearAllEmails()
{
    return addrbook::AddrMgrImpl::GetInstance().ClearAllEmails();
}

ResultCode AddressManager::UpdateGroup(const GroupDto& dto)
{
    return addrbook::AddrMgrImpl::GetInstance().UpdateGroup(dto);
}

std::pair<ResultCode, SearchEmailResult> AddressManager::SearchEmail(const std::string& keyword, uint32_t current_page, uint32_t page_size)
{
    return addrbook::AddrMgrImpl::GetInstance().SearchEmail(keyword, current_page, page_size);
}

ResultCode AddressManager::ImportEmails(const std::string& file_path, const ImportExportCallback& cb)
{
    return addrbook::AddrMgrImpl::GetInstance().ImportEmails(file_path, cb);
}

ResultCode AddressManager::ExportEmails(const ImportExportCallback& cb)
{
    std::string file_path = AddrMgrUtilities::GenerateTimestampedFileName(EMAIL_EXPORT_PREFIX, EXPORT_FILE_SUFFIX);
    return AddressManager::ExportEmails(file_path, cb);
}

ResultCode AddressManager::ExportEmails(const std::string& file_path, const ImportExportCallback& cb)
{
    return addrbook::AddrMgrImpl::GetInstance().ExportEmails(file_path, cb);
}

void AddressManager::Register(const std::shared_ptr<IAddressMgrDataObserver>& observer)
{
    addrbook::AddrMgrImpl::GetInstance().Register(observer);
}

void AddressManager::Unregister(const std::shared_ptr<IAddressMgrDataObserver>& observer)
{
    addrbook::AddrMgrImpl::GetInstance().Unregister(observer);
}