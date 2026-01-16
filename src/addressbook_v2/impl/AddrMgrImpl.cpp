#include "AddrMgrConfigDefs.h"
#include "AddrMgrImpl.h"
#include "AddrMgrLog.h"
#include "AddrMgrUtilities.h"
#include "CheckerProvider.h"
#include "CsvProcessor.h"
#include <algorithm>
#include <sys/stat.h>

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
        EventType event_type = event_sptr->GetType();
        if (EventType::EMail_Import == event_type)
        {
            ImportEmailsSync(event_sptr->GetFilePath(), event_sptr->GetCallback());
        }
        else if (EventType::EMail_Export == event_type)
        {
            ExportEmailsSync(event_sptr->GetFilePath(), event_sptr->GetCallback());
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

    do
    {
        if (rids.empty())
        {
            AB_LOG_E("empty rid list");
            break;
        }
        if (std::any_of(rids.cbegin(), rids.cend(), [](uint32_t rid) {
                return rid == 0;
            }))
        {
            AB_LOG_E("invalid rid");
            break;
        }
        if (AddrMgrUtilities::GetSortedUniqueRids(rids).size() != rids.size())
        {
            AB_LOG_E("duplicated rid");
            break;
        }
        std::lock_guard<std::mutex> lock(m_mtx);
        result = m_email_srv.RemoveEmail(rids);
    } while (false);
    return result;
}

ResultCode AddrMgrImpl::RemoveGroup(const std::vector<uint32_t>& rids)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    return m_email_srv.RemoveGroup(rids);
}

ResultCode AddrMgrImpl::UpdateEmail(const EmailDto& dto)
{
    ResultCode rsult = ResultCode::kInvalidParam;
    if (addrbook::CheckerProvider::GetInstance().Verify(dto))
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        rsult = m_email_srv.UpdateEmail(dto);
    }
    return rsult;
}

ResultCode AddrMgrImpl::UpdateGroup(const GroupDto& dto)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    return m_email_srv.UpdateGroup(dto);
}

std::pair<ResultCode, SearchEmailResult> AddrMgrImpl::SearchEmail(const std::string& keyword, uint32_t cur_page, uint32_t page_size)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    return m_email_srv.SearchEmail(keyword, cur_page, page_size);
}

ResultCode AddrMgrImpl::ImportEmails(const std::string& file_path, const ImportExportCallback& cb)
{
    ResultCode rsult = ResultCode::kInvalidParam;

    struct stat stat_buf = {};
    if (stat(file_path.c_str(), &stat_buf) == 0)
    {
        if (S_ISREG(stat_buf.st_mode) && stat_buf.st_size > 0)
        {
            m_evt_loop.PushEvent(std::make_shared<ImportExportEvent>(EventType::EMail_Import, file_path, cb));
            rsult = ResultCode::kSuccess;
        }
    }

    return rsult;
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

void AddrMgrImpl::ImportEmailsSync(const std::string& file_path, const ImportExportCallback& cb)
{
    addrbook::CsvReader csv_reader(file_path, {"邮件名字", "邮件地址", "邮件组名字"});
    std::vector<std::string> row;
    addrbook::CsvStatus status = CSV_ERROR_INIT_FAILED;
    bool result = true;
    while (result)
    {
        // 逐行读取
        status = csv_reader.ReadNextRow(row);
        if (status == addrbook::CSV_SUCCESS)
        {
            std::string group_names = row[2];
            std::vector<std::string> group_names_vec = AddrMgrUtilities::Split(group_names, ",");
            EmailDto dto(row[0], row[1], group_names_vec);
            if (addrbook::CheckerProvider::GetInstance().Verify(dto))
            {
                ResultCode code = m_email_srv.AddEmailAndGroup(dto);
                if (code != ResultCode::kSuccess)
                {
                    result = false;
                    break;
                }
            }
        }
        else if (status == addrbook::CSV_EOF)
        {
            break;
        }
        else
        {
            result = false;
            break;
        }
        if (cb)
        {
            cb(file_path, result);
            m_evt_dispatcher.Notify(ChangeType::kImportEmail);
        }
    }
}

void AddrMgrImpl::ExportEmailsSync(const std::string& file_path, const ImportExportCallback& cb)
{
    uint32_t cur_page = 0;
    uint32_t page_size = 10;
    uint32_t total_pages = 1;
    bool is_success = true;

    std::lock_guard<std::mutex> lock(m_mtx);
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
                item.emplace_back(dto.GetName());
                item.emplace_back(dto.GetAddress());
                item.emplace_back(AddrMgrUtilities::Join(dto.GetGroupNames(), ","));
                return item;
            });
            addrbook::CsvWriter csv_writer(file_path, {"邮件名字", "邮件地址", "邮件组名字"});
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
    if (cb)
    {
        cb(file_path, is_success);
    }
}

void AddrMgrImpl::ClearAllEmails()
{
    std::lock_guard<std::mutex> lock(m_mtx);
    return m_email_srv.ClearAllEmails();
}
}  // namespace addrbook