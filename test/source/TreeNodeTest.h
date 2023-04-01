#pragma once

#include <gtest/gtest.h>
#include "lib.hpp"

using namespace ArborMaster;

//TEST(TreeNodeTest, ChildInsert){
//  TreeNode parent;
//  TreeNode child1, child2, child3;
//  child1.name = "child1";
//  child2.name = "child2";
//  child3.name = "child3";
//  parent.insertChild(child1, 0);
//  parent.insertChild(child2, 1);
//  parent.insertChild(child3, 1);
//
//  ASSERT_EQ(parent.children.size(), 3);
//
//  EXPECT_EQ(parent.children[0]->name, "child1");
//  EXPECT_EQ(parent.children[2]->name, "child2");
//  EXPECT_EQ(parent.children[1]->name, "child3");
//}
//
//TEST(TreeNodeTest, ChildLimit){
//  TreeNode parent;
//  parent.childCap = 2;
//  TreeNode child1, child2, child3;
//  child1.name = "child1";
//  child2.name = "child2";
//  child3.name = "child3";
//  EXPECT_TRUE(parent.insertChild(child1, 0));
//  EXPECT_TRUE(parent.insertChild(child2, 1));
//  EXPECT_FALSE(parent.insertChild(child3, 2));
//
//  ASSERT_EQ(parent.children.size(), 2);
//
//  EXPECT_EQ(parent.children[0]->name, "child1");
//  EXPECT_EQ(parent.children[1]->name, "child2");
//}
//
//TEST(TreeNodeTest, RemoveChildren){
//  TreeNode parent;
//  TreeNode child1, child2, child3;
//  child1.name = "child1";
//  child2.name = "child2";
//  child3.name = "child3";
//  EXPECT_TRUE(parent.insertChild(child1, 0));
//  EXPECT_TRUE(parent.insertChild(child2, 1));
//  EXPECT_TRUE(parent.insertChild(child3, 2));
//
//  ASSERT_EQ(parent.children.size(), 3);
//
//  parent.removeChild(1);
//
//  EXPECT_EQ(parent.children.size(), 2);
//  EXPECT_EQ(parent.children[0]->name, "child1");
//  EXPECT_EQ(parent.children[1]->name, "child3");
//}
//
//TEST(TreeNodeTest, ChildReorder)
//{
//  TreeNode parent;
//  TreeNode child1, child2, child3;
//  child1.name = "child1";
//  child2.name = "child2";
//  child3.name = "child3";
//  parent.insertChild(child1, 0);
//  parent.insertChild(child2, 1);
//  parent.insertChild(child3, 2);
//  parent.reorderChild(0, 1);
//
//  ASSERT_EQ(parent.children.size(), 3);
//
//  EXPECT_EQ(parent.children[1]->name, "child1");
//  EXPECT_EQ(parent.children[0]->name, "child2");
//  EXPECT_EQ(parent.children[2]->name, "child3");
//}