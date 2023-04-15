#pragma once

#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include "lib.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

namespace ArborMaster {
class TreeExporterTest : public ::testing::Test
{
protected:
  TreeExporter exporter;
  EditorTree bt;
  NodeFactory nf;
  TreeNode child1, child2, child3;
  void SetUp() override
  {
    child1.name = "TreeNode1";
    child1.blackboardKeys.emplace("key1");
    child2.name = "TreeNode2";
    child2.blackboardKeys.emplace("key2");
    child3.name = "TreeNode3";
    child3.blackboardKeys.emplace("key3");
  }

  void TearDown() override {}

  void fillTree()
  {
    auto id = bt.getNewId();
    bt.m_editorNodes.emplace(id, EditorNode(child1, ImVec2(), id));
    id = bt.getNewId();
    bt.m_editorNodes.emplace(id, EditorNode(child2, ImVec2(), id));
    id = bt.getNewId();
    bt.m_editorNodes.emplace(id, EditorNode(child3, ImVec2(), id));

    EditorLink l;
    l.id = bt.getNewId();
    l.startId = bt.getNodeOutputId(bt.m_editorNodes[3]);
    l.endId = bt.getNodeInputId(bt.m_editorNodes[4]);
    bt.m_editorLinks.emplace(l.id, l);
    bt.m_adjList[bt.getLinkParent(l)].push_back(bt.getLinkChild(l));

    l.id = bt.getNewId();
    l.startId = bt.getNodeOutputId(bt.m_editorNodes[3]);
    l.endId = bt.getNodeInputId(bt.m_editorNodes[5]);
    bt.m_editorLinks.emplace(l.id, l);
    bt.m_adjList[bt.getLinkParent(l)].push_back(bt.getLinkChild(l));

    //Add root node
    bt.m_adjList[1] = { 3 };
  }

  void clearTree()
  {
    bt.m_editorId = 2;
    bt.m_editorLinks.clear();
    bt.m_editorNodes.clear();
    bt.m_adjList.clear();

    bt.m_adjList[1] = std::vector<int>();
  }

  void clearLinks()
  {
    std::vector<int> linkIds;
    linkIds.reserve(bt.m_editorLinks.size());
    for (auto& [id, link] : bt.m_editorLinks) {
      linkIds.push_back(id);
    }
    for (auto id : linkIds) {
      bt.deleteLink(bt.m_editorLinks[id]);
    }

  }
  const std::unordered_map<int, EditorNode>& getEditorNodes()
  {
    return bt.m_editorNodes;
  }
  std::unordered_set<int>& getFreeNodes()
  {
    return bt.m_freeNodes;
  }
  std::unordered_map<int, EditorLink>& getEditorLinks()
  {
    return bt.m_editorLinks;
  }
  std::unordered_map<int, std::vector<int>>& getAdjList()
  {
    return bt.m_adjList;
  }
};

TEST_F(TreeExporterTest, ExportTreeValid)
{
  fillTree();
  //Export tree
  bt.setPath("../../../test/assets/ExportTreeValid.json");
  exporter.setPath("../../../test/assets/ExportTreeValidExport.json");
  exporter.exportTree(bt);
  //Check file exists
  ASSERT_TRUE(std::filesystem::exists(exporter.getPath()));
  //Load json
  std::ifstream stream(exporter.getPath());
  json data;
  stream >> data;
  stream.close();
  
  //Check expected values
  EXPECT_EQ(data["debugPath"].get<std::string>(), std::filesystem::absolute(bt.getPath())) << "Exported debug path does not match bt design path";

  EXPECT_EQ(data["root"]["id"].get<int>(), 3) << "Root node id incorrect";
  EXPECT_EQ(data["root"]["name"].get<std::string>(), "TreeNode1") << "Root node name incorrect";

  ASSERT_EQ(data["root"]["children"].size(), 2) << "Unexpected number of children of root node";

  EXPECT_EQ(data["root"]["id"].get<int>(), 3) << "Root node id incorrect";
  EXPECT_EQ(data["root"]["name"].get<std::string>(), "TreeNode1") << "Root node name incorrect";

  EXPECT_EQ(data["root"]["children"][0]["id"].get<int>(), 4) << "Child 1 id incorrect";
  EXPECT_EQ(data["root"]["children"][0]["name"].get<std::string>(), "TreeNode2") << "Child 1 name incorrect";

  EXPECT_EQ(data["root"]["children"][1]["id"].get<int>(), 5) << "Child 2 id incorrect";
  EXPECT_EQ(data["root"]["children"][1]["name"].get<std::string>(), "TreeNode3") << "Child 1 name incorrect";

  std::filesystem::remove(exporter.getPath());
}

TEST_F(TreeExporterTest, ExportTreeEmpty)
{
  fillTree();
  clearTree();
  //Export tree
  bt.setPath("../../../test/assets/ExportTreeEmpty.json");
  exporter.setPath("../../../test/assets/ExportTreeEmptyExport.json");
  exporter.exportTree(bt);

  //Check file exists
  ASSERT_TRUE(std::filesystem::exists(exporter.getPath()));
  //Load json
  std::ifstream stream(exporter.getPath());
  json data;
  stream >> data;
  stream.close();

  EXPECT_EQ(data["debugPath"].get<std::string>(), std::filesystem::absolute(bt.getPath())) << "Exported debug path does not match bt design path";

  EXPECT_EQ(data.size(), 1) << "More than debug path present in empty tree export";
}

TEST_F(TreeExporterTest, ExportTreeFreeNodes)
{
  fillTree();
  clearLinks();
  //Export tree
  bt.setPath("../../../test/assets/ExportTreeFreeNodes.json");
  exporter.setPath("../../../test/assets/ExportTreeFreeNodesExport.json");
  exporter.exportTree(bt);

  //Check file exists
  ASSERT_TRUE(std::filesystem::exists(exporter.getPath()));
  //Load json
  std::ifstream stream(exporter.getPath()); 
  json data;
  stream >> data;
  stream.close();

  EXPECT_EQ(data["debugPath"].get<std::string>(), std::filesystem::absolute(bt.getPath())) << "Exported debug path does not match bt design path";

  EXPECT_EQ(data.size(), 2) << "Free node export should have debug path and root only";

  EXPECT_EQ(data["root"]["id"].get<int>(), 3) << "Root node id incorrect";
  EXPECT_EQ(data["root"]["name"].get<std::string>(), "TreeNode1") << "Root node name incorrect";
}

//TODO: Mock ImNodes::LoadCurrentEditorStateFromIniFile if at all possible
TEST_F(TreeExporterTest, SaveDesignValid)
{
  fillTree();
  bt.setPath("../../../test/assets/SaveTreeValid.json");

  exporter.saveDesign(bt, bt.getNewId() - 1);

  //Check files exists
  ASSERT_TRUE(std::filesystem::exists(bt.getPath())) << "Design json not present";


  //Load json
  std::ifstream stream(bt.getPath());
  json data;
  try {
    stream >> data;
  }
  catch (std::exception e) {
    std::cout << e.what() << std::endl;
  }
  
  stream.close();

  //Check expected values

  std::unordered_map<int, EditorNode> editorNodes;
  data["editorNodes"].get_to(editorNodes);
  EXPECT_EQ(editorNodes.size(), 3) << "Unexpected number of editorNodes saved";

  for (auto& [id, node] : editorNodes) {
    ASSERT_TRUE(getEditorNodes().contains(id)) << "Incorrect editorNodes saved";
  }

  std::unordered_map<intptr_t, std::string> nodeDefs;
  std::unordered_map<intptr_t, std::string> correctNodeDefs = {
    {(intptr_t) & child1, child1.name},
    {(intptr_t) & child2, child2.name},
    {(intptr_t) & child3, child3.name},
  };
  data["nodeDefs"].get_to(nodeDefs);
  EXPECT_EQ(nodeDefs.size(), 3) << "Unexpected number of node defs saved";

  for (auto& [id, node] : editorNodes) {
    intptr_t tn = reinterpret_cast<intptr_t>(node.treeNode);
    EXPECT_TRUE(nodeDefs.contains(tn)) << "Editor Node pointer not found in node definitions";
  }

  std::unordered_map<int, EditorLink> editorLinks;
  data["editorLinks"].get_to(editorLinks);

  std::unordered_map<int, std::vector<int>> adjList;
  data["adjList"].get_to(adjList);

  std::unordered_set<int> freeNodes;
  data["freeNodes"].get_to(freeNodes);

  EXPECT_EQ(bt.getNewId() - 2, data["currentId"].get<int>()) << "Incorrect current Id saved";
}

TEST_F(TreeExporterTest, SaveDesignEmpty)
{
  bt.setPath("../../../test/assets/SaveTreeEmpty.json");

  exporter.saveDesign(bt, bt.getNewId() - 1);

  //Check files exists
  ASSERT_TRUE(std::filesystem::exists(bt.getPath())) << "Design json not present";


  //Load json
  std::ifstream stream(bt.getPath());
  json data;
  try {
    stream >> data;
  }
  catch (std::exception e) {
    std::cout << e.what() << std::endl;
  }

  stream.close();

  //Check expected values

  std::unordered_map<int, EditorNode> editorNodes;
  data["editorNodes"].get_to(editorNodes);
  EXPECT_EQ(editorNodes.size(), 0) << "Unexpected number of editorNodes saved";

  std::unordered_map<intptr_t, std::string> nodeDefs;
  data["nodeDefs"].get_to(nodeDefs);
  EXPECT_EQ(nodeDefs.size(), 0) << "Unexpected number of node defs saved";

  for (auto& [id, node] : editorNodes) {
    intptr_t tn = reinterpret_cast<intptr_t>(node.treeNode);
    EXPECT_TRUE(nodeDefs.contains(tn)) << "Editor Node pointer not found in node definitions";
  }

  std::unordered_map<int, EditorLink> editorLinks;
  data["editorLinks"].get_to(editorLinks);
  EXPECT_EQ(editorLinks.size(), 0) << "Unexpected number of editor links saved";

  std::unordered_map<int, std::vector<int>> adjList;
  data["adjList"].get_to(adjList);
  EXPECT_EQ(adjList.size(), 0) << "Unexpected adjacency list saved";


  std::unordered_set<int> freeNodes;
  data["freeNodes"].get_to(freeNodes);
  EXPECT_EQ(freeNodes.size(), 0) << "Unexpected number of node defs saved";

  EXPECT_EQ(bt.getNewId() - 2, data["currentId"].get<int>()) << "Incorrect current Id saved";
}

TEST_F(TreeExporterTest, SaveDesignFreeNodes)
{
  fillTree();
  clearLinks();
  bt.setPath("../../../test/assets/SaveTreeFreeNodes.json");

  exporter.saveDesign(bt, bt.getNewId() - 1);

  //Check files exists
  ASSERT_TRUE(std::filesystem::exists(bt.getPath())) << "Design json not present";


  //Load json
  std::ifstream stream(bt.getPath());
  json data;
  try {
    stream >> data;
  }
  catch (std::exception e) {
    std::cout << e.what() << std::endl;
  }

  stream.close();

  //Check expected values

  std::unordered_map<int, EditorNode> editorNodes;
  data["editorNodes"].get_to(editorNodes);
  EXPECT_EQ(editorNodes.size(), 3) << "Unexpected number of editorNodes saved";

  for (auto& [id, node] : editorNodes) {
    ASSERT_TRUE(getEditorNodes().contains(id)) << "Incorrect editorNodes saved";
  }

  std::unordered_map<intptr_t, std::string> nodeDefs;
  std::unordered_map<intptr_t, std::string> correctNodeDefs = {
    {(intptr_t)&child1, child1.name},
    {(intptr_t)&child2, child2.name},
    {(intptr_t)&child3, child3.name},
  };
  data["nodeDefs"].get_to(nodeDefs);
  EXPECT_EQ(nodeDefs.size(), 3) << "Unexpected number of node defs saved";

  for (auto& [id, node] : editorNodes) {
    intptr_t tn = reinterpret_cast<intptr_t>(node.treeNode);
    EXPECT_TRUE(nodeDefs.contains(tn)) << "Editor Node pointer not found in node definitions";
  }

  std::unordered_map<int, EditorLink> editorLinks;
  data["editorLinks"].get_to(editorLinks);
  EXPECT_EQ(editorLinks.size(), 0) << "Unexpected number of editor links saved";

  std::unordered_map<int, std::vector<int>> adjList;
  data["adjList"].get_to(adjList);
  EXPECT_EQ(adjList.size(), 1) << "Unexpected adjacency list saved";

  std::unordered_set<int> freeNodes;
  data["freeNodes"].get_to(freeNodes);
  EXPECT_EQ(freeNodes.size(), 2) << "Unexpected free nodes saved";

  EXPECT_EQ(bt.getNewId() - 2, data["currentId"].get<int>()) << "Incorrect current Id saved";
}

TEST_F(TreeExporterTest, LoadDesignValid)
{
  std::vector treeNodeNames = {
    "SequenceNode",
    "WaitNode",
    "WanderTargetNode",
    "MoveNode",
    "WaitStartNode",
  };
  for (auto key : treeNodeNames) {
    nf.getNodes().emplace(key, TreeNode());
  }

  ASSERT_TRUE(exporter.loadDesign(bt, nf, "../../../test/assets/testDesign.json"));

  ASSERT_EQ(getAdjList().size(), 6);

  std::vector adjListKeys = { 1, 9, 3, 11, 5, 7 };
  for (auto key : adjListKeys) {
    ASSERT_TRUE(getAdjList().contains(key));
  }

  ASSERT_EQ(getEditorLinks().size(), 5);

  std::vector editorLinkKeys = { 4, 12, 6, 8, 10 };
  for (auto key : editorLinkKeys) {
    ASSERT_TRUE(getEditorLinks().contains(key));
  }
  ASSERT_EQ(getEditorNodes().size(), 5);

  std::vector editorNodeKeys = { 3, 11, 5, 7, 9 };
  for (auto key : editorNodeKeys) {
    ASSERT_TRUE(getEditorNodes().contains(key));
  }

  ASSERT_EQ(getFreeNodes().size(), 0);
}

TEST_F(TreeExporterTest, LoadDesignInvalid)
{
  ASSERT_FALSE(exporter.loadDesign(bt, nf, "../../../test/assets/testDesignInvalid.json"));

  ASSERT_EQ(getAdjList().size(), 0);

  ASSERT_EQ(getEditorLinks().size(), 0);
  ASSERT_EQ(getEditorNodes().size(), 0);
  ASSERT_EQ(getFreeNodes().size(), 0);
}

TEST_F(TreeExporterTest, LoadDesignEmpty)
{
  ASSERT_TRUE(exporter.loadDesign(bt, nf, "../../../test/assets/LoadTreeEmpty.json"));

  ASSERT_EQ(getAdjList().size(), 0);

  ASSERT_EQ(getEditorLinks().size(), 0);
  ASSERT_EQ(getEditorNodes().size(), 0);
  ASSERT_EQ(getFreeNodes().size(), 0);
}

TEST_F(TreeExporterTest, LoadDesignFreeNodes)
{
  std::vector treeNodeNames = {
    "TreeNode2",
    "TreeNode1",
    "TreeNode3",
  };
  for (auto key : treeNodeNames) {
    nf.getNodes().emplace(key, TreeNode());
  }

  ASSERT_TRUE(exporter.loadDesign(bt, nf, "../../../test/assets/LoadTreeFreeNodes.json"));

  ASSERT_EQ(getAdjList().size(), 1);

  ASSERT_TRUE(getAdjList().contains(1));

  ASSERT_EQ(getEditorLinks().size(), 0);

  ASSERT_EQ(getEditorNodes().size(), 3);

  std::vector editorNodeKeys = { 3, 4, 5 };
  for (auto key : editorNodeKeys) {
    ASSERT_TRUE(getEditorNodes().contains(key));
  }

  ASSERT_EQ(getFreeNodes().size(), 2);

  std::vector freeNodeKeys = { 4, 5 };
  for (auto key : freeNodeKeys) {
    ASSERT_TRUE(getFreeNodes().contains(key));
  }
}

TEST_F(TreeExporterTest, LoadDesignMismatch)
{
  std::vector treeNodeNames = {
    "NotSequenceNode",
    "NotWaitNode",
    "NotWanderTargetNode",
    "NotMoveNode",
    "NotWaitStartNode",
  };
  for (auto key : treeNodeNames) {
    nf.getNodes().emplace(key, TreeNode());
  }
  ASSERT_FALSE(exporter.loadDesign(bt, nf, "../../../test/assets/testDesign.json"));
}
}