#pragma once

#include <gtest/gtest.h>
#include "lib.hpp"

#include <string>
#include <unordered_set>

namespace ArborMaster {
  class EditorTreeTest : public ::testing::Test {
  public:
    EditorTree bt;
    TreeNode child1, child2, child3;
  protected:
    //Wrap private methods
    void updateBlackboard()
    {
      bt.updateBlackboard();
    }

    void updateTree()
    {
      bt.updateTree();
    }

    void deleteLink(EditorLink link)
    {
      bt.deleteLink(link);
    }

    void deleteEditorNode(EditorNode node)
    {
      bt.deleteEditorNode(node);
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
    }

    void clearTree()
    {
      bt.m_editorId = 2;
      bt.m_editorLinks.clear();
      bt.m_editorNodes.clear();
      bt.m_adjList.clear();
    }

    void SetUp() override
    {
      child1.name = "TreeNode1";
      child1.blackboardKeys.emplace("key1");
      child2.name = "TreeNode2";
      child2.blackboardKeys.emplace("key2");
      child3.name = "TreeNode3";
      child3.blackboardKeys.emplace("key3");

    }

    void TearDown() override
    {

    }
  };

  //Given an empty tree, updateTree should set the blackboard to be empty
  TEST_F(EditorTreeTest, updateBlackboardEmpty)
  {
    updateBlackboard();
    auto bb = bt.getBlackboard();
    ASSERT_TRUE(bb.data.empty()) << "Blackboard of empty tree is not empty";
  }

  //Given a tree with a number of nodes, updateTree should ensure blackboard has all keys found in the child nodes
  TEST_F(EditorTreeTest, updateBlackboardFull)
  {
    fillTree();
    updateBlackboard();

    std::unordered_set<std::string> assumedBB = { "key1", "key2", "key3" };
    for (const auto& key : assumedBB) {
      EXPECT_TRUE(bt.getBlackboard().data.contains(key)) << "Blackboard does not contain expected key" << key;
    }
  }

  //Delete a link from the tree
  TEST_F(EditorTreeTest, deleteLink)
  {
    fillTree();

    //Check # of links
    ASSERT_TRUE(getEditorLinks().size() == 2) << "Wrong number of initial links";

    //Check link exists
    auto it = getEditorLinks().begin();
    ASSERT_TRUE(it != getEditorLinks().end());
    EditorLink l = (*it).second;

    //Check adj list
    int parentId = bt.getLinkParent(l);
    int childId = bt.getLinkChild(l);

    ASSERT_TRUE(getAdjList().contains(parentId)) << "Initial link parent not in adj list";
    ASSERT_EQ(getAdjList().at(parentId).size(), 2) << "Initial link child id not in adj list";
    ASSERT_TRUE(getAdjList().at(parentId)[0] == childId) << "Initial link child in adj list has wrong id";

    //Delete Link
    deleteLink(l);

    //Check # of links
    ASSERT_EQ(getEditorLinks().size(), 1) << "Number of links not reduced after link is deleted";

    //Check link doesn't exist
    ASSERT_FALSE(getEditorLinks().contains(l.id)) << "Link still in editor links after deletion";

    //Check adj list is updated

    ASSERT_TRUE(getAdjList().contains(parentId)) << "Parent was removed from adj list when link";
    ASSERT_EQ(getAdjList().at(parentId).size(), 1) << "Removing link did not update adjacency list";

  }

  //Delete a link whose ID doesn't exist
  TEST_F(EditorTreeTest, deleteLinkNotExist)
  {
    fillTree();

    //Check # of links
    ASSERT_TRUE(getEditorLinks().size() == 2) << "Wrong number of initial links";

    //Check link exists
    auto it = getEditorLinks().begin();
    ASSERT_TRUE(it != getEditorLinks().end());
    EditorLink l = (*it).second;

    //Check adj list
    int parentId = bt.getLinkParent(l);
    int childId = bt.getLinkChild(l);

    ASSERT_TRUE(getAdjList().contains(parentId)) << "Initial link parent not in adj list";
    ASSERT_EQ(getAdjList().at(parentId).size(), 2) << "Initial link child id not in adj list";
    ASSERT_TRUE(getAdjList().at(parentId)[0] == childId) << "Initial link child in adj list has wrong id";

    l.id = -1;
    deleteLink(l);

    //Check # of links
    ASSERT_TRUE(getEditorLinks().size() == 2) << "Unintended link delete";

    //Check adj list
    ASSERT_TRUE(getAdjList().size() == 1) << "Unintended alteration of adj list";
  }

  //Delete a link from an empty tree
  TEST_F(EditorTreeTest, deleteLinkEmpty)
  {
    ASSERT_TRUE(getEditorLinks().empty()) << "Empty tree contains links";
    ASSERT_TRUE(getAdjList().empty()) << "Empty tree contains items in adj list";

    EditorLink l;
    l.id = 1;
    l.startId = 2;
    l.endId = 3;
    deleteLink(l);

    ASSERT_TRUE(getEditorLinks().empty()) << "Deletion created link(s)";
    ASSERT_TRUE(getAdjList().empty()) << "Deletion created adj list item(s)";

  }

  //Delete a Node from the tree
  TEST_F(EditorTreeTest, deleteNode)
  {
    fillTree();

    //Check # of nodes
    ASSERT_EQ(getEditorNodes().size(), 3) << "Wrong number of initial nodes";

    //Check Node exists
    ASSERT_TRUE(getEditorNodes().contains(3)) << "Root node not in editor nodes";
    EditorNode n = getEditorNodes().at(3);

    ASSERT_TRUE(getAdjList().contains(n.id)) << "Initial link parent not in adj list";
    ASSERT_EQ(getAdjList().at(n.id).size(), 2) << "Initial link child id not in adj list";
    ASSERT_TRUE(getFreeNodes().empty()) << "Initial freenode map contains items";

    //Delete Node
    deleteEditorNode(n);

    //Check # of nodes
    ASSERT_EQ(getEditorNodes().size(), 2) << "Incorrect number of nodes after deletion";

    //Check link doesn't exist
    ASSERT_FALSE(getEditorNodes().contains(n.id)) << "Node still in editor nodes map after deletion";

    //Check adj list is updated

    ASSERT_TRUE(getAdjList().empty()) << "Adjacency list is not empty afer single parent deleted";
    ASSERT_TRUE(getEditorLinks().empty()) << "Editor links not empty after single parent deleted";
    ASSERT_EQ(getFreeNodes().size(), 2) << "Child nodes not freed after parent deleted";
    ASSERT_FALSE(getFreeNodes().contains(n.id)) << "Deleted node found in free nodes";
  }

  //Delete a Node whose ID doesn't exist
  TEST_F(EditorTreeTest, deleteNodeNotExist)
  {
    fillTree();

    //Create Node, but don't add it to
    EditorNode n(child1, ImVec2(), bt.getNewId());
    EXPECT_EQ(getAdjList()[3].size(), 2) << "Beginning adj value is incorrect";

    //Delete Node
    deleteEditorNode(n);

    //Check lists empty
    EXPECT_EQ(getEditorLinks().size(), 2) << "Deletion of non-existing node altered link(s)";
    EXPECT_EQ(getAdjList().size(), 1) << "Deletion of non-existing node altered adj list";
    EXPECT_EQ(getAdjList()[3].size(), 2) << "Deletion of non-existing node altered adj list item(s)";

    EXPECT_TRUE(getFreeNodes().empty()) << "Deletion of non-existing node created free nodes";
    EXPECT_EQ(getEditorNodes().size(), 3) << "Deletion of non-existing node altered editor nodes";
  }

  //Delete a Node from an empty tree
  TEST_F(EditorTreeTest, deleteNodeEmpty)
  {
    //Check lists empty
    ASSERT_TRUE(getEditorLinks().empty()) << "Wrong number of initial links";
    ASSERT_TRUE(getAdjList().empty()) << "Adjacency list should be empty";

    ASSERT_TRUE(getFreeNodes().empty()) << "Free node list should be empty";;
    ASSERT_TRUE(getEditorNodes().empty()) << "Deletion created adj list item(s)";


    //Create Node
    EditorNode n(child1, ImVec2(), bt.getNewId());

    //Delete Node
    deleteEditorNode(n);

    //Check lists empty
    //Check lists empty
    ASSERT_TRUE(getEditorLinks().empty()) << "Deletion created link(s)";
    ASSERT_TRUE(getAdjList().empty()) << "Deletion created adj list item(s)";

    ASSERT_TRUE(getFreeNodes().empty()) << "Deletion created adj list item(s)";
    ASSERT_TRUE(getEditorNodes().empty()) << "Deletion created adj list item(s)";
  }
}