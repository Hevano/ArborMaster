#pragma once

#include <gtest/gtest.h>
#include "lib.h"

class BehaviourTreeTest : public ::testing::Test {
    BehaviourTree bt;
    void SetUp() override {
        TreeNode child1, child2, child3;
        child1.name = "child1";
        child1.blackboardKeys.emplace("key1");
        child2.name = "child2";
        child1.blackboardKeys.emplace("key2");
        child3.name = "child3";
        child1.blackboardKeys.emplace("key3");
        bt.root.blackboardKeys.emplace("key0");
        bt.root.addChild(child1, 0);
        bt.root.addChild(child2, 1);
        bt.root.addChild(child3, 2);
    }

    void TearDown() override{

    }
}

FRIEND_TEST_F(BehaviourTreeTest, ChildReorder)
{
  bt.updateBlackBoard();
  std::unordered_set<string> assumed
  bt.blackboard
}