#include "AddrCenterImpl.h"

AddrCenterImpl::AddrCenterImpl()
  : m_email_srv{}
  , m_evt_loop{std::bind(&AddrCenterImpl::EventHandler, this, std::placeholders::_1)}
  , m_mtx{}
{}

void AddrCenterImpl::EventHandler(const std::shared_ptr<IEvent>& evt_sptr)
{
    std::shared_ptr<ImportExportEvent> event_sptr = std::static_pointer_cast<ImportExportEvent>(evt_sptr);
    if (event_sptr)
    {
        if (EventType::EMail_Import == event_sptr->GetType())
        {
        }
    }
}

std::pair<ErrorCode, EmailDto> AddrCenterImpl::AddEmail(const EmailDto& dto)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    return m_email_srv.AddEmail(dto);
}

std::pair<ErrorCode, GroupDto> AddrCenterImpl::AddGroup(const GroupDto& dto)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    return m_email_srv.AddGroup(dto);
}

ErrorCode AddrCenterImpl::RemoveEmail(const std::vector<uint32_t>& rids)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    return m_email_srv.RemoveEmail(rids);
}

ErrorCode AddrCenterImpl::RemoveGroup(const std::vector<uint32_t>& rids)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    return m_email_srv.RemoveGroup(rids);
}

ErrorCode AddrCenterImpl::UpdateEmail(const EmailDto& dto)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    return m_email_srv.UpdateEmail(dto);
}

ErrorCode AddrCenterImpl::UpdateGroup(const GroupDto& dto)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    return m_email_srv.UpdateGroup(dto);
}

std::pair<ErrorCode, SearchResult> AddrCenterImpl::SearchEmail(const std::string& keyword, uint32_t current_page, uint32_t page_size)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    return m_email_srv.SearchEmail(keyword, current_page, page_size);
}

ErrorCode AddrCenterImpl::ImportEmail(const std::string& file_path, ImportFileType type)
{
    ErrorCode code = ErrorCode::kInvalidParam;
    if (ImportFileType::CSV == type)
    {
        m_evt_loop.PushEvent(std::make_shared<ImportExportEvent>(EventType::EMail_Import, file_path, type));
    }
    return code;
}