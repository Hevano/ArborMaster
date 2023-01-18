#include <gtest/gtest.h>


// Demonstrate some basic assertions.
TEST(YuckTest, BasicAssertions)
{
  EXPECT_STRNE("hello", "world");
}

// Demonstrate some basic assertions.
TEST(YuckTest, BasicAssertions3)
{
  EXPECT_STRNE("hello", "world");
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}