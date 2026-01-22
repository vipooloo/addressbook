#include "AddrMgrImpl.h"
#include "AddrMgrUtilities.h"
#include "AddressManager.h"

std::pair<ResultCode, EmailDto> AddressManager::CreateEmail(const EmailDto& dto)
{
    return addrbook::AddrMgrImpl::GetInstance().CreateEmail(dto);
}

ResultCode AddressManager::DeleteEmails(const std::vector<uint32_t>& ids)
{
    return addrbook::AddrMgrImpl::GetInstance().DeleteEmails(ids);
}

ResultCode AddressManager::DeleteAllEmails()
{
    return addrbook::AddrMgrImpl::GetInstance().DeleteAllEmails();
}

ResultCode AddressManager::UpdateEmail(const EmailDto& dto)
{
    return addrbook::AddrMgrImpl::GetInstance().UpdateEmail(dto);
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
    return addrbook::AddrMgrImpl::GetInstance().ExportEmails(file_path, cb);
}

/*---------------------------------------------------*/
std::pair<ResultCode, GroupDto> AddressManager::CreateGroup(const GroupDto& dto)
{
    return addrbook::AddrMgrImpl::GetInstance().CreateGroup(dto);
}

ResultCode AddressManager::DeleteGroups(const std::vector<uint32_t>& ids)
{
    return addrbook::AddrMgrImpl::GetInstance().DeleteGroups(ids);
}

ResultCode AddressManager::DeleteAllGroups()
{
    return addrbook::AddrMgrImpl::GetInstance().DeleteAllGroups();
}

ResultCode AddressManager::RemoveGroup(const std::vector<uint32_t>& ids)
{
    return addrbook::AddrMgrImpl::GetInstance().DeleteGroups(ids);
}

ResultCode AddressManager::UpdateGroup(const GroupDto& dto)
{
    return addrbook::AddrMgrImpl::GetInstance().UpdateGroup(dto);
}

std::pair<ResultCode, GroupPageResult> AddressManager::PageQueryGroup(const PageQueryParam& query_param)
{
    return addrbook::AddrMgrImpl::GetInstance().PageQueryGroup(query_param);
}
/*---------------------------------------------------*/
ResultCode AddressManager::Register(const std::shared_ptr<IAddressMgrDataObserver>& observer)
{
    return addrbook::AddrMgrImpl::GetInstance().Register(observer);
}

ResultCode AddressManager::Unregister(const std::shared_ptr<IAddressMgrDataObserver>& observer)
{
    return addrbook::AddrMgrImpl::GetInstance().Unregister(observer);
}