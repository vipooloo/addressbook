#include "AddrCenter.h"
#include "AddrCenterImpl.h"

std::pair<ErrorCode, EmailDto> AddrCenter::AddEmail(const EmailDto& dto)
{
    return AddrCenterImpl::GetInstance().GetEmailService().AddEmail(dto);
}

std::pair<ErrorCode, GroupDto> AddrCenter::AddGroup(const GroupDto& dto)
{
    return AddrCenterImpl::GetInstance().GetEmailService().AddGroup(dto);
}

ErrorCode AddrCenter::RemoveEmail(const std::vector<uint32_t>& rids)
{
    return AddrCenterImpl::GetInstance().GetEmailService().RemoveEmail(rids);
}

ErrorCode AddrCenter::RemoveGroup(const std::vector<uint32_t>& rids)
{
    return AddrCenterImpl::GetInstance().GetEmailService().RemoveGroup(rids);
}

ErrorCode AddrCenter::UpdateEmail(const EmailDto& dto)
{
    return AddrCenterImpl::GetInstance().GetEmailService().UpdateEmail(dto);
}

ErrorCode AddrCenter::UpdateGroup(const GroupDto& dto)
{
    return AddrCenterImpl::GetInstance().GetEmailService().UpdateGroup(dto);
}

std::pair<ErrorCode, SearchResult> AddrCenter::SearchEmail(const std::string& keyword, uint32_t current_page, uint32_t page_size)
{
    return AddrCenterImpl::GetInstance().GetEmailService().SearchEmail(keyword, current_page, page_size);
}