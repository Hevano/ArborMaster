#pragma once

#include <gtest/gtest.h>
#include "lib.hpp"

using namespace ArborMaster;

class BlackboardTest : public ::testing::Test
{
protected:
  Blackboard bb;
  void SetUp() override
  { 
	  
  }

  void TearDown() override {}
};

TEST_F(BlackboardTest, HasKey)
{
  bb.data.emplace("key1");
  EXPECT_TRUE(bb.hasKey("key1"));
}