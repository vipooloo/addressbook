#include "AddrMgrImpl.h"
#include "AddrMgrUtilities.h"
#include "AddressManager.h"

std::pair<ResultCode, EmailDto> AddressManager::CreateEmail(const EmailDto& dto)
{
    return addrbook::AddrMgrImpl::GetInstance().CreateEmail(dto);
}

std::pair<ResultCode, GroupDto> AddressManager::AddGroup(const GroupDto& dto)
{
    return addrbook::AddrMgrImpl::GetInstance().AddGroup(dto);
}

ResultCode AddressManager::DeleteEmails(const std::vector<uint32_t>& ids)
{
    return addrbook::AddrMgrImpl::GetInstance().DeleteEmails(ids);
}

ResultCode AddressManager::RemoveGroup(const std::vector<uint32_t>& ids)
{
    return addrbook::AddrMgrImpl::GetInstance().RemoveGroup(ids);
}

ResultCode AddressManager::UpdateEmail(const EmailDto& dto)
{
    return addrbook::AddrMgrImpl::GetInstance().UpdateEmail(dto);
}

ResultCode AddressManager::DeleteAllEmails()
{
    return addrbook::AddrMgrImpl::GetInstance().DeleteAllEmails();
}

ResultCode AddressManager::UpdateGroup(const GroupDto& dto)
{
    return addrbook::AddrMgrImpl::GetInstance().UpdateGroup(dto);
}

std::pair<ResultCode, EmailPageResult> AddressManager::PageQueryEmail(const PageQueryParam& query_param)
{
    return addrbook::AddrMgrImpl::GetInstance().PageQueryEmail(query_param);
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