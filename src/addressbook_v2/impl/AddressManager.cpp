#include "AddressManager.h"
#include "AddressMgrImpl.h"

std::pair<ResultCode, EmailDto> AddressManager::AddEmail(const EmailDto& dto)
{
    return AddressMgrImpl::GetInstance().AddEmail(dto);
}

std::pair<ResultCode, GroupDto> AddressManager::AddGroup(const GroupDto& dto)
{
    return AddressMgrImpl::GetInstance().AddGroup(dto);
}

ResultCode AddressManager::RemoveEmail(const std::vector<uint32_t>& rids)
{
    return AddressMgrImpl::GetInstance().RemoveEmail(rids);
}

ResultCode AddressManager::RemoveGroup(const std::vector<uint32_t>& rids)
{
    return AddressMgrImpl::GetInstance().RemoveGroup(rids);
}

ResultCode AddressManager::UpdateEmail(const EmailDto& dto)
{
    return AddressMgrImpl::GetInstance().UpdateEmail(dto);
}

ResultCode AddressManager::UpdateGroup(const GroupDto& dto)
{
    return AddressMgrImpl::GetInstance().UpdateGroup(dto);
}

std::pair<ResultCode, AddrCenterSearchResult> AddressManager::SearchEmail(const std::string& keyword, uint32_t current_page, uint32_t page_size)
{
    return AddressMgrImpl::GetInstance().SearchEmail(keyword, current_page, page_size);
}

ResultCode AddressManager::ImportEmails(const std::string& file_path, const ImportExportCallback& cb)
{
    return AddressMgrImpl::GetInstance().ImportEmails(file_path, cb);
}

ResultCode AddressManager::ExportEmails(const ImportExportCallback& cb)
{
    return AddressMgrImpl::GetInstance().ExportEmails("", cb);
}

ResultCode AddressManager::ExportEmails(const std::string& file_path, const ImportExportCallback& cb)
{
    return AddressMgrImpl::GetInstance().ExportEmails(file_path, cb);
}

void AddressManager::Register(const std::shared_ptr<IAddressDataObserver>& observer)
{
    AddressMgrImpl::GetInstance().Register(observer);
}

void AddressManager::Unregister(const std::shared_ptr<IAddressDataObserver>& observer)
{
    AddressMgrImpl::GetInstance().Unregister(observer);
}