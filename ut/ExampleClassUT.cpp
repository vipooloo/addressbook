#include "AddressManager.h"
#include "Common.h"

namespace {
class ExampleClassUT : public ::testing::Test
{
  protected:
    void SetUp() override {}
    void TearDown() override {}
    static void SetUpTestSuite() {}
    static void TearDownTestSuite() {}
};

TEST_F(ExampleClassUT, case1)
{
    EXPECT_TRUE(true);
}
}  // namespace