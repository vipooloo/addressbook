#include "gmock/gmock.h"
#include "gtest/gtest.h"

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
}
}  // namespace