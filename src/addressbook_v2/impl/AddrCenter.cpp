#include "AddrCenter.h"
#include "AddrCenterImpl.h"

std::pair<ResultCode, EmailDto> AddrCenter::AddEmail(const EmailDto& dto)
{
    return AddrCenterImpl::GetInstance().AddEmail(dto);
}

std::pair<ResultCode, GroupDto> AddrCenter::AddGroup(const GroupDto& dto)
{
    return AddrCenterImpl::GetInstance().AddGroup(dto);
}

ResultCode AddrCenter::RemoveEmail(const std::vector<uint32_t>& rids)
{
    return AddrCenterImpl::GetInstance().RemoveEmail(rids);
}

ResultCode AddrCenter::RemoveGroup(const std::vector<uint32_t>& rids)
{
    return AddrCenterImpl::GetInstance().RemoveGroup(rids);
}

ResultCode AddrCenter::UpdateEmail(const EmailDto& dto)
{
    return AddrCenterImpl::GetInstance().UpdateEmail(dto);
}

ResultCode AddrCenter::UpdateGroup(const GroupDto& dto)
{
    return AddrCenterImpl::GetInstance().UpdateGroup(dto);
}

std::pair<ResultCode, SearchResult> AddrCenter::SearchEmail(const std::string& keyword, uint32_t current_page, uint32_t page_size)
{
    return AddrCenterImpl::GetInstance().SearchEmail(keyword, current_page, page_size);
}

ResultCode AddrCenter::ImportEmails(const std::string& file_path, const ImportExportCallback& cb)
{
    return AddrCenterImpl::GetInstance().ImportEmails(file_path, cb);
}

ResultCode AddrCenter::ExportEmails(const ImportExportCallback& cb)
{
    return AddrCenterImpl::GetInstance().ExportEmails("", cb);
}

ResultCode AddrCenter::ExportEmails(const std::string& file_path, const ImportExportCallback& cb)
{
    return AddrCenterImpl::GetInstance().ExportEmails(file_path, cb);
}