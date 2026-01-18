#include "AddressManager.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {
class ConcreteAddressMgrDataObserver : public IAddressMgrDataObserver
{
  public:
    ConcreteAddressMgrDataObserver() = default;
    ~ConcreteAddressMgrDataObserver() = default;
    void OnChanged(ChangeType type) override
    {
        static_cast<void>(type);
    }

  private:
};
class ObserverUT : public ::testing::Test
{
  protected:
    void SetUp() override {}
    void TearDown() override {}
    static void SetUpTestSuite() {}
    static void TearDownTestSuite() {}
};

TEST_F(ObserverUT, Register_case1)
{
    auto observer = std::make_shared<ConcreteAddressMgrDataObserver>();

    ResultCode result = AddressManager::Register(observer);
    EXPECT_EQ(result, ResultCode::kSuccess);

    result = AddressManager::Register(observer);
    EXPECT_EQ(result, ResultCode::kAlreadyExist);

    result = AddressManager::Unregister(observer);
    EXPECT_EQ(result, ResultCode::kSuccess);

    result = AddressManager::Unregister(observer);
    EXPECT_EQ(result, ResultCode::kNotFound);
}

TEST_F(ObserverUT, Register_case2)
{
    ResultCode result = AddressManager::Register(nullptr);
    EXPECT_EQ(result, ResultCode::kInvalidParam);

    result = AddressManager::Unregister(nullptr);
    EXPECT_EQ(result, ResultCode::kInvalidParam);
}

}  // namespace