#include "AddrMgrConfigDefs.h"
#include "AddrMgrImpl.h"
#include "AddrMgrLog.h"
#include "AddrMgrUtilities.h"
#include "CheckerProvider.h"
#include "CsvProcessor.h"
#include <algorithm>

namespace addrbook {
AddrMgrImpl::AddrMgrImpl()
  : m_evt_loop{std::bind(&AddrMgrImpl::EventHandler, this, std::placeholders::_1)}
  , m_mtx{}
  , m_evt_dispatcher{}
  , m_email_srv{m_evt_dispatcher}
{
    m_evt_loop.Start();
}

AddrMgrImpl::~AddrMgrImpl()
{
    m_evt_loop.Stop();
}

void AddrMgrImpl::EventHandler(const std::shared_ptr<IEvent>& evt_sptr)
{
    std::shared_ptr<ImportExportEvent> event_sptr = std::static_pointer_cast<ImportExportEvent>(evt_sptr);
    if (event_sptr)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
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
            std::string file_name = event_sptr->GetFilePath();
            std::string out_file_name = file_name.empty() ? AddrMgrUtilities::GenerateTimestampedFileName(EMAIL_EXPORT_PREFIX, EXPORT_FILE_SUFFIX) : file_name;
            while (cur_page < total_pages)
            {
                ++cur_page;
                std::pair<ResultCode, SearchEmailResult> result = m_email_srv.SearchEmail("", cur_page, page_size);
                if (result.first == ResultCode::kSuccess)
                {
                    SearchEmailResult& export_res = result.second;
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
                    addrbook::CsvWriter csv_writer(out_file_name, {"列A", "列B"});
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
                cb(out_file_name, is_success);
            }
        }
    }
}

std::pair<ResultCode, EmailDto> AddrMgrImpl::AddEmail(const EmailDto& dto)
{
    if (addrbook::CheckerProvider::GetInstance().Verify(dto))
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        return m_email_srv.AddEmail(dto);
    }
    else
    {
        return std::make_pair(ResultCode::kInvalidParam, dto);
    }
}

std::pair<ResultCode, GroupDto> AddrMgrImpl::AddGroup(const GroupDto& dto)
{
    if (addrbook::CheckerProvider::GetInstance().Verify(dto))
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        return m_email_srv.AddGroup(dto);
    }
    else
    {
        return std::make_pair(ResultCode::kInvalidParam, dto);
    }
}

ResultCode AddrMgrImpl::RemoveEmail(const std::vector<uint32_t>& rids)
{
    ResultCode result = ResultCode::kInvalidParam;

    if (rids.empty())
    {
        AB_LOG_E("empty rid list");
    }
    else if (std::any_of(rids.cbegin(), rids.cend(), [](uint32_t rid) {
                 return rid == 0;
             }))
    {
        AB_LOG_E("invalid rid");
    }
    else
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        result = m_email_srv.RemoveEmail(rids);
    }

    return result;
}

ResultCode AddrMgrImpl::RemoveGroup(const std::vector<uint32_t>& rids)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    return m_email_srv.RemoveGroup(rids);
}

ResultCode AddrMgrImpl::UpdateEmail(const EmailDto& dto)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    return m_email_srv.UpdateEmail(dto);
}

ResultCode AddrMgrImpl::UpdateGroup(const GroupDto& dto)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    return m_email_srv.UpdateGroup(dto);
}

std::pair<ResultCode, SearchEmailResult> AddrMgrImpl::SearchEmail(const std::string& keyword, uint32_t current_page, uint32_t page_size)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    return m_email_srv.SearchEmail(keyword, current_page, page_size);
}

ResultCode AddrMgrImpl::ImportEmails(const std::string& file_path, const ImportExportCallback& cb)
{
    m_evt_loop.PushEvent(std::make_shared<ImportExportEvent>(EventType::EMail_Import, file_path, cb));
    return ResultCode::kSuccess;
}

ResultCode AddrMgrImpl::ExportEmails(const std::string& file_path, const ImportExportCallback& cb)
{
    m_evt_loop.PushEvent(std::make_shared<ImportExportEvent>(EventType::EMail_Export, file_path, cb));
    return ResultCode::kSuccess;
}

void AddrMgrImpl::Register(const std::shared_ptr<IAddressMgrDataObserver>& observer)
{
    m_evt_dispatcher.Register(observer);
}

void AddrMgrImpl::Unregister(const std::shared_ptr<IAddressMgrDataObserver>& observer)
{
    m_evt_dispatcher.Unregister(observer);
}

}  // namespace addrbook