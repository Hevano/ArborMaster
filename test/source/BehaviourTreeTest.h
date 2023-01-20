#pragma once

#include <gtest/gtest.h>
#include "lib.hpp"

#include <string>
#include <unordered_set>

class BehaviourTreeTest : public ::testing::Test {
protected:
    BehaviourTree bt;
    void SetUp() override {
        TreeNode child1, child2, child3;
        child1.name = "child1";
        child1.blackboardKeys.emplace("key1");
        child2.name = "child2";
        child2.blackboardKeys.emplace("key2");
        child3.name = "child3";
        child3.blackboardKeys.emplace("key3");
        bt.getRoot().blackboardKeys.emplace("key0");
        bt.getRoot().insertChild(child1, 0);
        bt.getRoot().insertChild(child2, 1);
        bt.getRoot().insertChild(child3, 2);
    }

    void TearDown() override{

    }
};

TEST_F(BehaviourTreeTest, BlackboardUpdates)
{
  bt.updateBlackboard();
  std::unordered_set<std::string> assumedBB = {"key1", "key2", "key3", "key0"};
  for (const auto& key : assumedBB) {
      EXPECT_TRUE(bt.getBlackboard().data.contains(key));
  }

  TreeNode child4;
  child4.name = "child4";
  child4.blackboardKeys.emplace("key4");

  ASSERT_EQ(bt.getRoot().children.size(), 3);

  bt.getRoot().children[0]->insertChild(child4);
  
  assumedBB.emplace("key4");

  for (const auto& key : assumedBB) {
      EXPECT_TRUE(bt.getBlackboard().data.contains(key));
  }
}