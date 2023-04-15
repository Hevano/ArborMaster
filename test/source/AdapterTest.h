#pragma once

#include <gtest/gtest.h>
#include "lib.hpp"
#include "adapter/Adapter.h"

namespace ArborMasterAdapter {
class AdapterTest : public ::testing::Test
{
protected:
  Adapter adapter;
  void SetUp() override {}

  void TearDown() override {}
};


TEST_F(AdapterTest, LoadTreeValid)
{
  std::string debugPath;
  auto root_ptr = adapter.loadTree("../../../test/assets/testDesignExport.json", debugPath);

  EXPECT_EQ(debugPath, "C:\\Users\\Evano\\source\\repos\\ArborMaster\\test\\assets\\wanderDesign.json");

  ASSERT_NE(root_ptr.get(), nullptr) << "Root pointer is null";

  EXPECT_EQ(root_ptr->name, "SequenceNode") << "Root node has incorrect name";
  EXPECT_EQ(root_ptr->nodeId, 3) << "Root node has incorrect id";
  ASSERT_EQ(root_ptr->children.size(), 4) << "Root node has incorrect number of children";

  std::vector<std::pair<int, std::string>> childNodeData = {
    {5, "WanderTargetNode"},
    {7, "MoveNode"},
    {9, "WaitStartNode"},
    {11, "WaitNode"}
  };

  for (int i = 0; i < root_ptr->children.size(); i++) {
    EXPECT_EQ(root_ptr->children[i]->name, childNodeData[i].second) << "Child node has incorrect name";
    EXPECT_EQ(root_ptr->children[i]->nodeId, childNodeData[i].first) << "Child node has incorrect id";
    ASSERT_EQ(root_ptr->children[i]->children.size(), 0) << "Node has incorrect number of children";
  }
}

TEST_F(AdapterTest, LoadTreeFileNotFound)
{
  std::string debugPath;
  auto root_ptr = adapter.loadTree("../../../test/assets/FileNotFound", debugPath);

  ASSERT_EQ(root_ptr.get(), nullptr) << "Root pointer is not null";
}

TEST_F(AdapterTest, LoadTreeInvalid)
{
  std::string debugPath;
  auto root_ptr = adapter.loadTree("../../../test/assets/malformedDesignExport.json", debugPath);

  ASSERT_EQ(root_ptr.get(), nullptr) << "Root pointer is not null";
}

TEST_F(AdapterTest, GetSubTreeValid)
{
  json subtree;
  subtree["id"] = 1;
  subtree["name"] = "TestNodeName";
  auto node = adapter.getSubTree(subtree);

  ASSERT_EQ(subtree["id"], node->nodeId) << "Node id not parsed correctly";
  ASSERT_EQ(subtree["name"], node->name) << "Name not parsed correctly";
  ASSERT_EQ(0, node->children.size()) << "Name not parsed correctly";
}

TEST_F(AdapterTest, GetSubTreeInvalid)
{
  json subtree;
  subtree["id"] = "IncorrectStringValue";
  subtree["name"] = 1337;

  auto node = adapter.getSubTree(subtree);
  ASSERT_EQ(node.get(), nullptr);
}

TEST_F(AdapterTest, GetSubTreeNested)
{
  json subtree;
  subtree["id"] = 1;
  subtree["name"] = "TestNodeName";

  auto str = subtree.dump();

  subtree["children"].push_back(json::parse(str));
  subtree["children"][0]["id"] = 2;
  subtree["children"][0]["name"] = "Child1Name";

  subtree["children"][0]["children"].push_back(json::parse(str));
  subtree["children"][0]["children"][0]["id"] = 3;
  subtree["children"][0]["children"][0]["name"] = "Child2Name";

  auto node = adapter.getSubTree(subtree);
  ASSERT_NE(node.get(), nullptr);


  ASSERT_EQ(subtree["id"], node->nodeId) << "Node id not parsed correctly";
  ASSERT_EQ(subtree["name"], node->name) << "Name not parsed correctly";
  ASSERT_EQ(1, node->children.size()) << "Name not parsed correctly";

  ASSERT_EQ(2, node->children[0]->nodeId) << "Node id not parsed correctly";
  ASSERT_EQ("Child1Name", node->children[0]->name) << "Name not parsed correctly";
  ASSERT_EQ(1, node->children[0]->children.size()) << "Name not parsed correctly";

  ASSERT_EQ(3, node->children[0]->children[0]->nodeId) << "Node id not parsed correctly";
  ASSERT_EQ("Child2Name", node->children[0]->children[0]->name) << "Name not parsed correctly";
  ASSERT_EQ(0, node->children[0]->children[0]->children.size()) << "Name not parsed correctly";
}

TEST_F(AdapterTest, GetSubTreeEmpty)
{
  json subtree;
  auto node = adapter.getSubTree(subtree);
  ASSERT_EQ(node.get(), nullptr);
}
}



