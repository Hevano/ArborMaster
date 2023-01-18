#pragma once

#include <gtest/gtest.h>
#include "lib.h"

// Demonstrate some basic assertions.
TEST(TreeNodeTest, ChildReorder)
{
  TreeNode parent;
  TreeNode child1, child2, child3;
  child1.name = "child1";
  child2.name = "child2";
  child3.name = "child3";
  t.addChild(child1, 0);
  t.addChild(child2, 1);
  t.addChild(child3, 2);
  t.reorderChild(firstChild, 1);

  EXPECT_STRE(t.children[1].name, "child1");
  EXPECT_STRE(t.children[0].name, "child2");
  EXPECT_STRE(t.children[2].name, "child3");
}

TEST(TreeNodeTest, ChildInsert){
  TreeNode parent;
  TreeNode child1, child2, child3;
  child1.name = "child1";
  child2.name = "child2";
  child3.name = "child3";
  t.addChild(child1, 0);
  t.addChild(child2, 1);
  t.addChild(child3, 1);

  EXPECT_STRE(t.children[0].name, "child1");
  EXPECT_STRE(t.children[2].name, "child2");
  EXPECT_STRE(t.children[1].name, "child3");
}

TEST(TreeNodeTest, ChildLimit){
  TreeNode parent;
  parent.childCap = 2;
  TreeNode child1, child2, child3;
  child1.name = "child1";
  child2.name = "child2";
  child3.name = "child3";
  EXPECT_TRUE(t.addChild(child1, 0));
  EXPECT_TRUE(t.addChild(child2, 1));
  EXPECT_FALSE(t.addChild(child3, 2));

  EXPECT_EQ(parent.children.size(), 2);
  EXPECT_STRE(t.children[0].name, "child1");
  EXPECT_STRE(t.children[1].name, "child2");
}

TEST(TreeNodeTest, RemoveChildren){
  TreeNode parent;
  TreeNode child1, child2, child3;
  child1.name = "child1";
  child2.name = "child2";
  child3.name = "child3";
  EXPECT_TRUE(t.addChild(child1, 0));
  EXPECT_TRUE(t.addChild(child2, 1));
  EXPECT_TRUE(t.addChild(child3, 2));

  parent.removeChild(1);

  EXPECT_EQ(parent.children.size(), 2);
  EXPECT_STRE(t.children[0].name, "child1");
  EXPECT_STRE(t.children[1].name, "child3");
}