#include "AddrCenterImpl.h"

AddrCenterImpl::AddrCenterImpl()
  : m_email_srv{}
  , m_evt_loop{std::bind(&AddrCenterImpl::EventHandler, this, std::placeholders::_1)}
{}

void AddrCenterImpl::EventHandler(const std::shared_ptr<IEvent>& evt_sptr)
{
}

std::pair<ErrorCode, EmailDto> AddrCenterImpl::AddEmail(const EmailDto& dto)
{
    return m_email_srv.AddEmail(dto);
}

std::pair<ErrorCode, GroupDto> AddrCenterImpl::AddGroup(const GroupDto& dto)
{
    return m_email_srv.AddGroup(dto);
}

ErrorCode AddrCenterImpl::RemoveEmail(const std::vector<uint32_t>& rids)
{
    return m_email_srv.RemoveEmail(rids);
}

ErrorCode AddrCenterImpl::RemoveGroup(const std::vector<uint32_t>& rids)
{
    return m_email_srv.RemoveGroup(rids);
}

ErrorCode AddrCenterImpl::UpdateEmail(const EmailDto& dto)
{
    return m_email_srv.UpdateEmail(dto);
}

ErrorCode AddrCenterImpl::UpdateGroup(const GroupDto& dto)
{
    return m_email_srv.UpdateGroup(dto);
}

std::pair<ErrorCode, SearchResult> AddrCenterImpl::SearchEmail(const std::string& keyword, uint32_t current_page, uint32_t page_size)
{
    return m_email_srv.SearchEmail(keyword, current_page, page_size);
}