#include "AddrCenterImpl.h"
#include "CsvProcessor.h"

AddrCenterImpl::AddrCenterImpl()
  : m_email_srv{}
  , m_evt_loop{std::bind(&AddrCenterImpl::EventHandler, this, std::placeholders::_1)}
  , m_mtx{}
{
    m_evt_loop.Start();
}

AddrCenterImpl::~AddrCenterImpl()
{
    m_evt_loop.Stop();
}

void AddrCenterImpl::EventHandler(const std::shared_ptr<IEvent>& evt_sptr)
{
    std::shared_ptr<ImportExportEvent> event_sptr = std::static_pointer_cast<ImportExportEvent>(evt_sptr);
    if (event_sptr)
    {
        EventType event_type = event_sptr->GetType();
        if (EventType::EMail_Import == event_type)
        {
        }
        else if (EventType::EMail_Export == event_type)
        {
            std::pair<ErrorCode, SearchResult> result = m_email_srv.SearchEmail("", 1, 11110);
            SearchResult& export_res = result.second;
            const std::vector<EmailDto>& records = export_res.GetRecords();
            std::vector<std::vector<std::string>> items;
            for (const EmailDto& dto : records)
            {
                std::vector<std::string> item;
                const std::string& name = dto.GetName();
                const std::string& email = dto.GetAddress();
                item.emplace_back(name);
                item.emplace_back(email);
                items.emplace_back(item);
            }
            CsvWriter csv_writer(event_sptr->GetFilePath(), {"列A", "列B"});
            csv_writer.WriteBatch(items);
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

ErrorCode AddrCenterImpl::ImportEmails(const std::string& file_path, FileType type)
{
    ErrorCode code = ErrorCode::kInvalidParam;
    if (FileType::CSV == type)
    {
        m_evt_loop.PushEvent(std::make_shared<ImportExportEvent>(EventType::EMail_Import, file_path, type));
    }
    return code;
}

ErrorCode AddrCenterImpl::ExportEmails(const std::string& file_path, FileType type)
{
    ErrorCode code = ErrorCode::kInvalidParam;
    if (FileType::CSV == type)
    {
        m_evt_loop.PushEvent(std::make_shared<ImportExportEvent>(EventType::EMail_Export, file_path, type));
    }
    return code;
}
