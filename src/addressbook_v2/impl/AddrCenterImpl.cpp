#include "AddrCenterImpl.h"
#include "AddrCenterLog.h"
#include "AddressBookConfigDefs.h"
#include "AddressCenterUtilities.h"
#include "CsvProcessor.h"
#include <algorithm>

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
            uint32_t cur_page = 0;
            uint32_t page_size = 10;
            uint32_t total_pages = 1;
            bool is_success = true;
            std::string file_name = event_sptr->GetFilePath().empty() ? AddressCenterUtilities::GenerateTimestampedFileName(EMAIL_EXPORT_PREFIX, EXPORT_FILE_SUFFIX) : event_sptr->GetFilePath();
            while (cur_page < total_pages)
            {
                ++cur_page;
                std::pair<ErrorCode, SearchResult> result = m_email_srv.SearchEmail("", cur_page, page_size);
                if (result.first == ErrorCode::kSuccess)
                {
                    SearchResult& export_res = result.second;
                    const std::vector<EmailDto>& records = export_res.GetRecords();
                    std::vector<std::vector<std::string>> items;
                    std::transform(records.cbegin(), records.cend(), std::back_inserter(items), [](const EmailDto& dto) {
                        std::vector<std::string> item;
                        const std::string& name = dto.GetName();
                        const std::string& email = dto.GetAddress();
                        item.emplace_back(name);
                        item.emplace_back(email);
                        return item;
                    });
                    CsvWriter csv_writer(file_name, {"列A", "列B"});
                    csv_writer.WriteBatch(items);
                    total_pages = export_res.GetTotalPages();
                }
                else
                {
                    is_success = false;
                    AB_LOG_E("export email failed, code: %u", static_cast<uint32_t>(result.first));
                    break;
                }
            }
            ImportExportCallback cb = event_sptr->GetCallback();
            if (cb)
            {
                cb(file_name, is_success);
            }
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

ErrorCode AddrCenterImpl::ImportEmails(const std::string& file_path, const ImportExportCallback& cb)
{
    m_evt_loop.PushEvent(std::make_shared<ImportExportEvent>(EventType::EMail_Import, file_path, cb));
    return ErrorCode::kSuccess;
}

ErrorCode AddrCenterImpl::ExportEmails(const std::string& file_path, const ImportExportCallback& cb)
{
    m_evt_loop.PushEvent(std::make_shared<ImportExportEvent>(EventType::EMail_Export, file_path, cb));
    return ErrorCode::kSuccess;
    ;
}
