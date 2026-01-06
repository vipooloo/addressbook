#include "AddrMgrImpl.h"
#include "AddressManager.h"

std::pair<ResultCode, EmailDto> AddressManager::AddEmail(const EmailDto& dto)
{
    return AddrMgrImpl::GetInstance().AddEmail(dto);
}

std::pair<ResultCode, GroupDto> AddressManager::AddGroup(const GroupDto& dto)
{
    return AddrMgrImpl::GetInstance().AddGroup(dto);
}

ResultCode AddressManager::RemoveEmail(const std::vector<uint32_t>& rids)
{
    return AddrMgrImpl::GetInstance().RemoveEmail(rids);
}

ResultCode AddressManager::RemoveGroup(const std::vector<uint32_t>& rids)
{
    return AddrMgrImpl::GetInstance().RemoveGroup(rids);
}

ResultCode AddressManager::UpdateEmail(const EmailDto& dto)
{
    return AddrMgrImpl::GetInstance().UpdateEmail(dto);
}

ResultCode AddressManager::UpdateGroup(const GroupDto& dto)
{
    return AddrMgrImpl::GetInstance().UpdateGroup(dto);
}

std::pair<ResultCode, SearchEmailResult> AddressManager::SearchEmail(const std::string& keyword, uint32_t current_page, uint32_t page_size)
{
    return AddrMgrImpl::GetInstance().SearchEmail(keyword, current_page, page_size);
}

ResultCode AddressManager::ImportEmails(const std::string& file_path, const ImportExportCallback& cb)
{
    return AddrMgrImpl::GetInstance().ImportEmails(file_path, cb);
}

ResultCode AddressManager::ExportEmails(const ImportExportCallback& cb)
{
    return AddrMgrImpl::GetInstance().ExportEmails("", cb);
}

ResultCode AddressManager::ExportEmails(const std::string& file_path, const ImportExportCallback& cb)
{
    return AddrMgrImpl::GetInstance().ExportEmails(file_path, cb);
}

void AddressManager::Register(const std::shared_ptr<IAddressMgrDataObserver>& observer)
{
    AddrMgrImpl::GetInstance().Register(observer);
}

void AddressManager::Unregister(const std::shared_ptr<IAddressMgrDataObserver>& observer)
{
    AddrMgrImpl::GetInstance().Unregister(observer);
}