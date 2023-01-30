#pragma once

#include <gtest/gtest.h>
#include "lib.hpp"

using namespace ArborMaster;

class NodeFactoryTest : public ::testing::Test
{
protected:
  NodeFactory nf;
  void SetUp() override
  { 
	  TreeNode t;
    t.name = "node1";
	  t.childCap = 1;
	  //nf.defineNode(t);
  }

  void TearDown() override {}
};

TEST_F(NodeFactoryTest, CreationTest)
{
  TreeNode t = nf.createNode("node1");
  EXPECT_EQ(t.name, "node1");
  EXPECT_EQ(t.childCap, 1);
  EXPECT_EQ(t.children.size(), 0);
}