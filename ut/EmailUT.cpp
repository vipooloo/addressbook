#include "AddressManager.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

namespace {
std::mutex mtx;
std::condition_variable cv;
bool ready = false;
void WaitReady()
{
    std::unique_lock<std::mutex> lk(mtx);
    cv.wait(lk, [] {
        return ready;
    });
    ready = false;
}
void NotifyReady()
{
    {
        std::lock_guard<std::mutex> lk(mtx);
        ready = true;
    }
    cv.notify_all();
}
class ConcreteAddressMgrDataObserver : public IAddressMgrDataObserver
{
  public:
    explicit ConcreteAddressMgrDataObserver(const std::function<bool(ChangeType)>& callback)
      : m_callback(callback)
    {}
    ~ConcreteAddressMgrDataObserver()
    {
        EXPECT_GT(m_change_count, 0u);
    }
    void OnChanged(ChangeType type) override
    {
        ++m_change_count;
        EXPECT_TRUE(m_callback != nullptr);
        if (m_callback)
        {
            bool result = m_callback(type);
            EXPECT_TRUE(result) << "事件为:" << static_cast<int>(type);
        }
    }
    void SetCallback(const std::function<bool(ChangeType)>& callback)
    {
        m_callback = callback;
    }

  private:
    std::function<bool(ChangeType)> m_callback;
    uint32_t m_change_count{0};
};
static bool IsFileEqualByDiff(const std::string& f1,
                              const std::string& f2)
{
    std::string cmd = "diff " + f1 + " " + f2;
    FILE* fp = popen(cmd.c_str(), "r");
    if (!fp)
        return false;

    int ch = fgetc(fp);
    pclose(fp);

    return ch == EOF;  // 没有任何输出
}

class EmailUT : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        ClearAllData();
    }
    void TearDown() override
    {
        ClearAllData();
    }
    static void SetUpTestSuite()
    {
    }
    static void TearDownTestSuite()
    {
    }
    void ClearAllData()
    {
        std::function<bool(ChangeType)> callback = [](ChangeType type) {
            return type == ChangeType::DeleteAllEmails;
        };
        auto observer = std::make_shared<ConcreteAddressMgrDataObserver>(callback);
        AddressManager::Register(observer);

        AddressManager::DeleteAllEmails();
        uint32_t cur_page = 1;
        uint32_t page_size = 10;
        auto result = AddressManager::PageQueryEmail(PageQueryParam("", cur_page, page_size));
        ASSERT_EQ(result.first, ResultCode::kSuccess);
        ASSERT_EQ(result.second.GetTotalRecords(), 0u);
        ASSERT_EQ(result.second.GetTotalPages(), 0u);
        ASSERT_EQ(result.second.GetCurrentPage(), cur_page);
        ASSERT_EQ(result.second.GetPageSize(), page_size);

        AddressManager::Unregister(observer);
    }
};

TEST_F(EmailUT, AddEmail_case1)
{
    std::function<bool(ChangeType)> callback = [](ChangeType type) {
        return type == ChangeType::CreateEmail;
    };
    auto observer = std::make_shared<ConcreteAddressMgrDataObserver>(callback);
    AddressManager::Register(observer);

    std::vector<EmailDto> add_email_cases = {
        {"alice@example.com", "Alice"},
        {"bob@example.com", "Bob"},
        {"charlie@example.com", "Charlie"},
    };
    std::vector<EmailDto> output_email_cases;
    for (const EmailDto& dto : add_email_cases)
    {
        auto result = AddressManager::CreateEmail(dto);
        ASSERT_EQ(result.first, ResultCode::kSuccess);
        ASSERT_GT(result.second.GetRid(), 0u);
        ASSERT_EQ(result.second.GetName(), dto.GetName());
        ASSERT_EQ(result.second.GetAddress(), dto.GetAddress());
        output_email_cases.emplace_back(result.second);
    }
    uint32_t cur_page = 1;
    uint32_t page_size = 10;
    auto result = AddressManager::PageQueryEmail(PageQueryParam("", cur_page, page_size));
    ASSERT_EQ(result.first, ResultCode::kSuccess);
    const std::vector<EmailDto>& query_results = result.second.GetRecords();
    EXPECT_EQ(result.second.GetTotalRecords(), add_email_cases.size());
    for (uint32_t i = 0; i < add_email_cases.size(); ++i)
    {
        EXPECT_EQ(query_results[i].GetRid(), output_email_cases[i].GetRid());
        EXPECT_EQ(query_results[i].GetName(), output_email_cases[i].GetName());
        EXPECT_EQ(query_results[i].GetAddress(), output_email_cases[i].GetAddress());
        EXPECT_EQ(query_results[i].GetName(), add_email_cases[i].GetName());
        EXPECT_EQ(query_results[i].GetAddress(), add_email_cases[i].GetAddress());
    }
    AddressManager::Unregister(observer);
}

TEST_F(EmailUT, RemoveEmail_case1)
{
    std::vector<EmailDto> add_email_cases = {
        {"alice@example.com", "Alice"},
        {"bob@example.com", "Bob"},
    };
    std::vector<EmailDto> output_email_cases;
    for (const EmailDto& dto : add_email_cases)
    {
        auto result = AddressManager::CreateEmail(dto);
        ASSERT_EQ(result.first, ResultCode::kSuccess);
        ASSERT_GT(result.second.GetRid(), 0u);
        ASSERT_EQ(result.second.GetName(), dto.GetName());
        ASSERT_EQ(result.second.GetAddress(), dto.GetAddress());
        output_email_cases.emplace_back(result.second);
    }
    uint32_t cur_page = 1;
    uint32_t page_size = 10;
    auto result = AddressManager::PageQueryEmail(PageQueryParam("", cur_page, page_size));
    ASSERT_EQ(result.first, ResultCode::kSuccess);
    const std::vector<EmailDto>& query_results = result.second.GetRecords();
    EXPECT_EQ(result.second.GetTotalRecords(), add_email_cases.size());
    std::vector<uint32_t> rids_to_remove;
    for (uint32_t i = 0; i < add_email_cases.size(); ++i)
    {
        EXPECT_EQ(query_results[i].GetRid(), output_email_cases[i].GetRid());
        rids_to_remove.emplace_back(query_results[i].GetRid());
    }

    std::function<bool(ChangeType)> callback = [](ChangeType type) {
        return type == ChangeType::DeleteEmails;
    };
    auto observer = std::make_shared<ConcreteAddressMgrDataObserver>(callback);
    AddressManager::Register(observer);

    ResultCode remove_result = AddressManager::DeleteEmails(rids_to_remove);
    EXPECT_EQ(remove_result, ResultCode::kSuccess);
    result = AddressManager::PageQueryEmail(PageQueryParam("", cur_page, page_size));
    ASSERT_EQ(result.first, ResultCode::kSuccess);
    EXPECT_EQ(result.second.GetTotalRecords(), 0u);

    AddressManager::Unregister(observer);
}

TEST_F(EmailUT, UpdateEmail_case1)
{
    {
        ResultCode result = AddressManager::UpdateEmail(EmailDto());
        EXPECT_EQ(result, ResultCode::kInvalidParam);
    }
    {
        ResultCode result = AddressManager::UpdateEmail(EmailDto{9999, "nonexist", "nonexist"});
        EXPECT_EQ(result, ResultCode::kNotFound);
    }
}

TEST_F(EmailUT, UpdateEmail_case2)
{
    AddressManager::CreateEmail(EmailDto("alice@example.com", "Alice"));
    uint32_t cur_page = 1;
    uint32_t page_size = 10;
    auto result = AddressManager::PageQueryEmail(PageQueryParam(cur_page, page_size));
    ASSERT_EQ(result.first, ResultCode::kSuccess);
    const std::vector<EmailDto>& query_results = result.second.GetRecords();
    EXPECT_EQ(result.second.GetTotalRecords(), 1u);
    EXPECT_GT(query_results[0].GetRid(), 0u);
    EXPECT_EQ(query_results[0].GetName(), "Alice");
    EXPECT_EQ(query_results[0].GetAddress(), "alice@example.com");

    std::function<bool(ChangeType)> callback = [](ChangeType type) {
        return type == ChangeType::UpdateEmail;
    };
    auto observer = std::make_shared<ConcreteAddressMgrDataObserver>(callback);
    AddressManager::Register(observer);

    ResultCode update_result = AddressManager::UpdateEmail(EmailDto(query_results[0].GetRid(), "new_alice@example.com", "new_Alice Updated"));
    EXPECT_EQ(update_result, ResultCode::kSuccess);
    result = AddressManager::PageQueryEmail(PageQueryParam(cur_page, page_size));
    ASSERT_EQ(result.first, ResultCode::kSuccess);
    const std::vector<EmailDto>& updated_query_results = result.second.GetRecords();
    EXPECT_EQ(result.second.GetTotalRecords(), 1u);
    EXPECT_EQ(updated_query_results[0].GetRid(), query_results[0].GetRid());
    EXPECT_EQ(updated_query_results[0].GetName(), "new_Alice Updated");
    EXPECT_EQ(updated_query_results[0].GetAddress(), "new_alice@example.com");

    AddressManager::Unregister(observer);
}

TEST_F(EmailUT, QueryEmail_case1)
{
    PageQueryParam query_param("nonexist", 1, 10);
    auto result = AddressManager::PageQueryEmail(query_param);
    ASSERT_EQ(result.first, ResultCode::kSuccess);
    EXPECT_EQ(result.second.GetTotalRecords(), 0u);
    EXPECT_EQ(result.second.GetTotalPages(), 0u);
    EXPECT_EQ(result.second.GetCurrentPage(), query_param.GetCurPage());
    EXPECT_EQ(result.second.GetPageSize(), query_param.GetPageSize());
}

TEST_F(EmailUT, QueryEmail_case2)
{
    PageQueryParam query_param(1, 10);
    auto result = AddressManager::PageQueryEmail(query_param);
    ASSERT_EQ(result.first, ResultCode::kSuccess);
    EXPECT_EQ(result.second.GetTotalRecords(), 0u);
    EXPECT_EQ(result.second.GetTotalPages(), 0u);
    EXPECT_EQ(result.second.GetCurrentPage(), query_param.GetCurPage());
    EXPECT_EQ(result.second.GetPageSize(), query_param.GetPageSize());
}

TEST_F(EmailUT, ImportEmails_case1)
{
    std::vector<EmailDto> add_email_cases = {
        {"alice@example.com", "Alice"},
        {"bob@example.com", "Bob"},
    };
    std::vector<EmailDto> output_email_cases;
    for (const EmailDto& dto : add_email_cases)
    {
        auto result = AddressManager::CreateEmail(dto);
        ASSERT_EQ(result.first, ResultCode::kSuccess);
        ASSERT_GT(result.second.GetRid(), 0u);
        ASSERT_EQ(result.second.GetName(), dto.GetName());
        ASSERT_EQ(result.second.GetAddress(), dto.GetAddress());
        output_email_cases.emplace_back(result.second);
    }

    std::string csv_file_name;
    ImportExportCallback export_callback = [&](const std::string& file_name, bool is_success) {
        EXPECT_TRUE(is_success);
        EXPECT_FALSE(file_name.empty());
        csv_file_name = file_name;
        EXPECT_EQ(file_name.substr(file_name.find_last_of('.') + 1), "csv");
        NotifyReady();
    };
    ResultCode export_result1 = AddressManager::ExportEmails(export_callback);
    EXPECT_EQ(export_result1, ResultCode::kSuccess);

    WaitReady();
    ResultCode delete_result = AddressManager::DeleteAllEmails();
    EXPECT_EQ(delete_result, ResultCode::kSuccess);

    std::function<bool(ChangeType)> callback = [](ChangeType type) {
        return type == ChangeType::ImportEmails;
    };
    auto observer = std::make_shared<ConcreteAddressMgrDataObserver>(callback);
    AddressManager::Register(observer);
    ResultCode import_result = AddressManager::ImportEmails(csv_file_name, [](const std::string& file_name, bool is_success) {
        static_cast<void>(&file_name);
        EXPECT_TRUE(is_success);
        NotifyReady();
    });
    EXPECT_EQ(import_result, ResultCode::kSuccess);
    WaitReady();

    {
        uint32_t cur_page = 1;
        uint32_t page_size = 10;
        auto result = AddressManager::PageQueryEmail(PageQueryParam(cur_page, page_size));
        ASSERT_EQ(result.first, ResultCode::kSuccess);
        const std::vector<EmailDto>& query_results = result.second.GetRecords();
        EXPECT_EQ(result.second.GetTotalRecords(), add_email_cases.size());
        for (uint32_t i = 0; i < add_email_cases.size(); ++i)
        {
            EXPECT_NE(query_results[i].GetRid(), output_email_cases[i].GetRid());
            EXPECT_EQ(query_results[i].GetName(), output_email_cases[i].GetName());
            EXPECT_EQ(query_results[i].GetAddress(), output_email_cases[i].GetAddress());
        }
    }

    std::string csv_file_name2;
    ImportExportCallback export_callback2 = [&csv_file_name2](const std::string& file_name, bool is_success) {
        EXPECT_TRUE(is_success);
        csv_file_name2 = file_name;
        NotifyReady();
    };
    ResultCode export_result2 = AddressManager::ExportEmails(export_callback2);
    EXPECT_EQ(export_result2, ResultCode::kSuccess);
    WaitReady();
    EXPECT_TRUE(IsFileEqualByDiff(csv_file_name, csv_file_name2));
    AddressManager::Unregister(observer);
}
}  // namespace