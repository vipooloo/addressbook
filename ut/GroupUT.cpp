#include "AddressManager.h"
#include "Common.h"

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
class GroupUT : public ::testing::Test
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
            return type == ChangeType::DeleteAllGroups;
        };
        auto observer = std::make_shared<ConcreteAddressMgrDataObserver>(callback);
        AddressManager::Register(observer);

        AddressManager::DeleteAllGroups();
        uint32_t cur_page = 1;
        uint32_t page_size = 10;
        auto result = AddressManager::PageQueryGroup(PageQueryParam("", cur_page, page_size));
        ASSERT_EQ(result.first, ResultCode::kSuccess);
        ASSERT_EQ(result.second.GetTotalRecords(), 0u);
        ASSERT_EQ(result.second.GetTotalPages(), 0u);
        ASSERT_EQ(result.second.GetCurrentPage(), cur_page);
        ASSERT_EQ(result.second.GetPageSize(), page_size);

        AddressManager::Unregister(observer);
    }
};

TEST_F(GroupUT, case1)
{
    std::function<bool(ChangeType)> callback = [](ChangeType type) {
        return type == ChangeType::CreateGroup;
    };
    auto observer = std::make_shared<ConcreteAddressMgrDataObserver>(callback);
    AddressManager::Register(observer);

    std::vector<GroupDto> add_group_set = {
        GroupDto{"Alice Group"},
        GroupDto{"Bob Group"},
    };
    std::vector<GroupDto> output_group_vec;
    for (const GroupDto& dto : add_group_set)
    {
        auto result = AddressManager::CreateGroup(dto);
        ASSERT_EQ(result.first, ResultCode::kSuccess);
        ASSERT_GT(result.second.GetRid(), 0u);
        ASSERT_EQ(result.second.GetGroupName(), dto.GetGroupName());
        output_group_vec.emplace_back(result.second);
    }
    uint32_t cur_page = 1;
    uint32_t page_size = 10;
    auto result = AddressManager::PageQueryGroup(PageQueryParam("", cur_page, page_size));
    ASSERT_EQ(result.first, ResultCode::kSuccess);
    const std::vector<GroupDto>& query_results = result.second.GetRecords();
    EXPECT_EQ(result.second.GetTotalRecords(), add_group_set.size());
    for (uint32_t i = 0; i < add_group_set.size(); ++i)
    {
        EXPECT_EQ(query_results[i].GetRid(), output_group_vec[i].GetRid());
        EXPECT_EQ(query_results[i].GetGroupName(), output_group_vec[i].GetGroupName());
        EXPECT_EQ(query_results[i].GetGroupName(), add_group_set[i].GetGroupName());
    }
    AddressManager::Unregister(observer);
}
TEST_F(GroupUT, RemoveGroup_case1)
{
    std::vector<GroupDto> add_group_set = {
        GroupDto{"Alice Group"},
        GroupDto{"Bob Group"},
    };
    std::vector<GroupDto> output_group_vec;
    for (const GroupDto& dto : add_group_set)
    {
        auto result = AddressManager::CreateGroup(dto);
        ASSERT_EQ(result.first, ResultCode::kSuccess);
        ASSERT_GT(result.second.GetRid(), 0u);
        ASSERT_EQ(result.second.GetGroupName(), dto.GetGroupName());
        output_group_vec.emplace_back(result.second);
    }
    uint32_t cur_page = 1;
    uint32_t page_size = 10;
    auto result = AddressManager::PageQueryGroup(PageQueryParam("", cur_page, page_size));
    ASSERT_EQ(result.first, ResultCode::kSuccess);
    const std::vector<GroupDto>& query_results = result.second.GetRecords();
    EXPECT_EQ(result.second.GetTotalRecords(), add_group_set.size());
    std::vector<uint32_t> rids_to_remove;
    for (uint32_t i = 0; i < add_group_set.size(); ++i)
    {
        EXPECT_EQ(query_results[i].GetRid(), output_group_vec[i].GetRid());
        rids_to_remove.emplace_back(query_results[i].GetRid());
    }

    std::function<bool(ChangeType)> callback = [](ChangeType type) {
        return type == ChangeType::DeleteGroups;
    };
    auto observer = std::make_shared<ConcreteAddressMgrDataObserver>(callback);
    AddressManager::Register(observer);

    ResultCode remove_result = AddressManager::DeleteGroups(rids_to_remove);
    EXPECT_EQ(remove_result, ResultCode::kSuccess);
    result = AddressManager::PageQueryGroup(PageQueryParam("", cur_page, page_size));
    ASSERT_EQ(result.first, ResultCode::kSuccess);
    EXPECT_EQ(result.second.GetTotalRecords(), 0u);

    AddressManager::Unregister(observer);
}

TEST_F(GroupUT, UpdateGroup_case1)
{
    {
        ResultCode result = AddressManager::UpdateGroup(GroupDto());
        EXPECT_EQ(result, ResultCode::kInvalidParam);
    }
    {
        ResultCode result = AddressManager::UpdateGroup(GroupDto{12322, "nonexist", {}});
        EXPECT_EQ(result, ResultCode::kNotFound);
    }
}

TEST_F(GroupUT, UpdateGroup_case2)
{
    AddressManager::CreateGroup(GroupDto("Alice Group"));
    uint32_t cur_page = 1;
    uint32_t page_size = 10;
    auto result = AddressManager::PageQueryGroup(PageQueryParam(cur_page, page_size));
    ASSERT_EQ(result.first, ResultCode::kSuccess);
    const std::vector<GroupDto>& query_results = result.second.GetRecords();
    EXPECT_EQ(result.second.GetTotalRecords(), 1u);
    EXPECT_GT(query_results[0].GetRid(), 0u);
    EXPECT_EQ(query_results[0].GetGroupName(), "Alice Group");

    std::function<bool(ChangeType)> callback = [](ChangeType type) {
        return type == ChangeType::UpdateGroup;
    };
    auto observer = std::make_shared<ConcreteAddressMgrDataObserver>(callback);
    AddressManager::Register(observer);

    ResultCode update_result = AddressManager::UpdateGroup(GroupDto(query_results[0].GetRid(), "Alice Group Updated", {}));
    EXPECT_EQ(update_result, ResultCode::kSuccess);
    result = AddressManager::PageQueryGroup(PageQueryParam(cur_page, page_size));
    ASSERT_EQ(result.first, ResultCode::kSuccess);
    const std::vector<GroupDto>& updated_query_results = result.second.GetRecords();
    EXPECT_EQ(result.second.GetTotalRecords(), 1u);
    EXPECT_EQ(updated_query_results[0].GetRid(), query_results[0].GetRid());
    EXPECT_EQ(updated_query_results[0].GetGroupName(), "Alice Group Updated");
    AddressManager::Unregister(observer);
}

TEST_F(GroupUT, QueryGroup_case1)
{
    PageQueryParam query_param("nonexist", 1, 10);
    auto result = AddressManager::PageQueryGroup(query_param);
    ASSERT_EQ(result.first, ResultCode::kSuccess);
    EXPECT_EQ(result.second.GetTotalRecords(), 0u);
    EXPECT_EQ(result.second.GetTotalPages(), 0u);
    EXPECT_EQ(result.second.GetCurrentPage(), query_param.GetCurPage());
    EXPECT_EQ(result.second.GetPageSize(), query_param.GetPageSize());
}

TEST_F(GroupUT, QueryGroup_case2)
{
    PageQueryParam query_param(1, 10);
    auto result = AddressManager::PageQueryGroup(query_param);
    ASSERT_EQ(result.first, ResultCode::kSuccess);
    EXPECT_EQ(result.second.GetTotalRecords(), 0u);
    EXPECT_EQ(result.second.GetTotalPages(), 0u);
    EXPECT_EQ(result.second.GetCurrentPage(), query_param.GetCurPage());
    EXPECT_EQ(result.second.GetPageSize(), query_param.GetPageSize());
}
}  // namespace