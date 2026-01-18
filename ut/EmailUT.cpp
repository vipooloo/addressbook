#include "AddressManager.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {

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
    const std::vector<EmailDto>& search_results = result.second.GetRecords();
    EXPECT_EQ(result.second.GetTotalRecords(), add_email_cases.size());
    for (uint32_t i = 0; i < add_email_cases.size(); ++i)
    {
        EXPECT_EQ(search_results[i].GetRid(), output_email_cases[i].GetRid());
        EXPECT_EQ(search_results[i].GetName(), output_email_cases[i].GetName());
        EXPECT_EQ(search_results[i].GetAddress(), output_email_cases[i].GetAddress());
        EXPECT_EQ(search_results[i].GetName(), add_email_cases[i].GetName());
        EXPECT_EQ(search_results[i].GetAddress(), add_email_cases[i].GetAddress());
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
    const std::vector<EmailDto>& search_results = result.second.GetRecords();
    EXPECT_EQ(result.second.GetTotalRecords(), add_email_cases.size());
    std::vector<uint32_t> rids_to_remove;
    for (uint32_t i = 0; i < add_email_cases.size(); ++i)
    {
        EXPECT_EQ(search_results[i].GetRid(), output_email_cases[i].GetRid());
        rids_to_remove.emplace_back(search_results[i].GetRid());
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
}  // namespace