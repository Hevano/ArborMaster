#pragma once

#include <gtest/gtest.h>
#include "lib.hpp"

namespace ArborMaster {

class NodeImporterTest : public ::testing::Test
{
protected:
  NodeImporter importer;
  void SetUp() override
  { 
	  //importer.setPath("../../../test/assets/NodeDefTest.cpp");
  }

  void TearDown() override {}

  TreeNode importString(const std::string& src)
  {
    return importer.importNode(src);
  }
}; 

TEST_F(NodeImporterTest, ImportNodeValid)
{
  auto tn = importString("//[ArborMaster]ImportNodeValidName|21|");
  EXPECT_EQ(tn.name, "ImportNodeValidName") << "Wrong name imported";
  EXPECT_EQ(tn.childCap, 21) << "Wrong child cap imported";
}

TEST_F(NodeImporterTest, ImportNodeValidBlackboard)
{
  auto tn = importString("//[ArborMaster]ImportNodeValidBlackboardName|3|BlackBoardKeyAlpha|BlackBoardKeyBeta|");
  EXPECT_EQ(tn.name, "ImportNodeValidBlackboardName") << "Wrong name imported";
  EXPECT_EQ(tn.childCap, 3) << "Wrong child cap imported";
  EXPECT_TRUE(tn.blackboardKeys.contains("BlackBoardKeyAlpha")) << "Did not import first blackboard key";
  EXPECT_TRUE(tn.blackboardKeys.contains("BlackBoardKeyBeta")) << "Did not import second blackboard key";
}

TEST_F(NodeImporterTest, ImportNodeEmpty)
{
  try {
    auto tn = importString("");
    FAIL() << "Importing invalid node string did not throw an exception";
  }
  catch (std::exception e) {
    std::string error_msg(e.what());
    EXPECT_EQ(error_msg, "Source string not flagged") << "Exception thrown from empty string not what was expected.";
  }
}

TEST_F(NodeImporterTest, ImportNodeInvalid)
{
  try {
    auto tn = importString("dfghdfgjdfghjdfg\nasdfasd\nsadfasdf\n");
    FAIL() << "Importing invalid node string did not throw an exception";
  }
  catch (std::exception e) {
    std::string error_msg(e.what());
    EXPECT_EQ(error_msg, "Source string not flagged") << "Exception thrown from empty string not what was expected.";
  }

  try {
    auto tn = importString("//[ArborMaster]");
    FAIL() << "Importing invalid node string did not throw an exception";
  }
  catch (std::exception e) {
    std::string error_msg(e.what());
    EXPECT_EQ(error_msg, "Not enough fields specified") << "Exception thrown from empty string not what was expected.";
  }
}

TEST_F(NodeImporterTest, ImportNodesValid)
{
  importer.setPath("../../../test/assets/NodeDefTest.cpp");
  EXPECT_EQ(importer.getPath(), "../../../test/assets/NodeDefTest.cpp") << "Path not set correctly";

  std::unordered_map<std::string, TreeNode> importedNodes;

  importer.importAll(importedNodes);

  //Check map entries are correct
  ASSERT_EQ(importedNodes.size(), 4) << "Incorrect number of nodes imported";
  ASSERT_TRUE(importedNodes.contains("NodeName1"));
  ASSERT_TRUE(importedNodes.contains("NodeName2"));
  ASSERT_TRUE(importedNodes.contains("NodeName3"));
  ASSERT_TRUE(importedNodes.contains("NodeName4"));

  //Check Treenode values are correct
  ASSERT_EQ(importedNodes["NodeName1"].name, "NodeName1") << "Incorrect node name used";
  ASSERT_EQ(importedNodes["NodeName1"].childCap, 0) << "incorrect child cap used";
  EXPECT_TRUE(importedNodes["NodeName1"].blackboardKeys.contains("blackboardkey1")) << "Did not import correct blackboard keys";

  ASSERT_EQ(importedNodes["NodeName2"].name, "NodeName2") << "Incorrect node name used";
  ASSERT_EQ(importedNodes["NodeName2"].childCap, 1) << "incorrect child cap used";
  EXPECT_TRUE(importedNodes["NodeName2"].blackboardKeys.contains("blackboardkey1")) << "Did not import correct blackboard keys";
  EXPECT_TRUE(importedNodes["NodeName2"].blackboardKeys.contains("blackboardkey2")) << "Did not import correct blackboard keys";

  ASSERT_EQ(importedNodes["NodeName3"].name, "NodeName3") << "Incorrect node name used";
  ASSERT_EQ(importedNodes["NodeName3"].childCap, 2) << "incorrect child cap used";
  EXPECT_TRUE(importedNodes["NodeName3"].blackboardKeys.contains("blackboardkey1")) << "Did not import correct blackboard keys";
  EXPECT_TRUE(importedNodes["NodeName3"].blackboardKeys.contains("blackboardkey2")) << "Did not import correct blackboard keys";
  EXPECT_TRUE(importedNodes["NodeName3"].blackboardKeys.contains("blackboardkey3")) << "Did not import correct blackboard keys";

  ASSERT_EQ(importedNodes["NodeName4"].name, "NodeName4") << "Incorrect node name used";
  ASSERT_EQ(importedNodes["NodeName4"].childCap, 3) << "incorrect child cap used";
  EXPECT_TRUE(importedNodes["NodeName4"].blackboardKeys.contains("blackboardkey1")) << "Did not import correct blackboard keys";
  EXPECT_TRUE(importedNodes["NodeName4"].blackboardKeys.contains("blackboardkey2")) << "Did not import correct blackboard keys";
  EXPECT_TRUE(importedNodes["NodeName4"].blackboardKeys.contains("blackboardkey3")) << "Did not import correct blackboard keys";
  EXPECT_TRUE(importedNodes["NodeName4"].blackboardKeys.contains("blackboardkey4")) << "Did not import correct blackboard keys";
}

TEST_F(NodeImporterTest, ImportNodesEmpty)
{
  importer.setPath("../../../test/assets/NodeDefTestEmpty.cpp");
  EXPECT_EQ(importer.getPath(), "../../../test/assets/NodeDefTestEmpty.cpp") << "Path not set correctly";

  std::unordered_map<std::string, TreeNode> importedNodes;

  importer.importAll(importedNodes);

  ASSERT_EQ(importedNodes.size(), 0) << "Incorrect number of nodes imported";
}

TEST_F(NodeImporterTest, ImportNodesInvalid)
{
  importer.setPath("../../../test/assets/NodeDefTestInvalid.cpp");
  EXPECT_EQ(importer.getPath(), "../../../test/assets/NodeDefTestInvalid.cpp") << "Path not set correctly";

  std::unordered_map<std::string, TreeNode> importedNodes;

  importer.importAll(importedNodes);

  ASSERT_EQ(importedNodes.size(), 0) << "Incorrect number of nodes imported";
}

TEST_F(NodeImporterTest, ImportNodesBadPath)
{
  importer.setPath("../../../test/assets/FileNotFound.cpp");
  EXPECT_EQ(importer.getPath(), "../../../test/assets/FileNotFound.cpp") << "Path not set correctly";

  std::unordered_map<std::string, TreeNode> importedNodes;

  importer.importAll(importedNodes);

  ASSERT_EQ(importedNodes.size(), 0) << "Incorrect number of nodes imported";
}

}