#pragma once

#include <gtest/gtest.h>
#include "lib.hpp"
#include "adapter/Debugger.h"

namespace ArborMasterAdapter {
  class DebuggerTest : public ::testing::Test
  {
  protected:
    Debugger debugger;
    void SetUp() override {}

    void TearDown() override {}

    Debugger::actorid_map_type* getActors()
    {
      return debugger.m_actorIdMap.get();
    }

    Debugger::bb_map_type* getBlackboard()
    {
      return debugger.m_blackBoardMap.get();
    }

    bool checkBlackboardValue(const std::string& key, const std::string& value)
    {
      std::string bbValue(getBlackboard()->at(Debugger::char_string(key.begin(), key.end(), *debugger.m_charAllocator)).c_str());
      return bbValue == value;
    }

    bool checkBlackboardContains(const std::string& key)
    {
      return getBlackboard()->contains(Debugger::char_string(key.begin(), key.end(), *debugger.m_charAllocator));
    }
  };

  TEST_F(DebuggerTest, InitFirst)
  {
    debugger.init();
    std::unique_ptr<DebugManager> dm(DebugManager::createInstance());
    ASSERT_NE(dm.get(), nullptr) << "Debug manager should not be null";
  }

  TEST_F(DebuggerTest, InitSecond)
  {
    std::unique_ptr<DebugManager> dm(DebugManager::createInstance());
    ASSERT_EQ(dm.get(), nullptr) << "Debug manager should be null";
    debugger.init();
    dm.reset(DebugManager::createInstance());
    ASSERT_NE(dm.get(), nullptr) << "Debug manager should not be null";
  }

  TEST_F(DebuggerTest, TickEmptyQueue)
  {
    debugger.init();
    auto currentActorId = debugger.getCurrentActorId();
    ASSERT_FALSE(debugger.tick());
    ASSERT_EQ(currentActorId, debugger.getCurrentActorId()) << "Actor Id changed";
  }

  TEST_F(DebuggerTest, ChangeCurrentActor)
  {
    debugger.init();
    std::unique_ptr<DebugManager> dm(DebugManager::createInstance());
    auto initialActorId = debugger.getCurrentActorId();
    dm->selectActor(1);
    ASSERT_TRUE(debugger.tick());
    ASSERT_NE(initialActorId, debugger.getCurrentActorId()) << "Actor Id wasn't changed";
    ASSERT_EQ(1, debugger.getCurrentActorId()) << "Actor Id incorrect";
  }

  TEST_F(DebuggerTest, UpdateNodeStatusValid)
  {
    debugger.init();
    std::unique_ptr<DebugManager> dm(DebugManager::createInstance());
    dm->selectActor(1);
    ASSERT_TRUE(debugger.tick());

    debugger.updateNodeStatus(0, 1, 1);

    unsigned int nodeId = 1337;
    unsigned int actorId = 1337;
    unsigned int status = 1337;
    ASSERT_TRUE(dm->getNodeUpdates(nodeId, actorId, status));

    ASSERT_EQ(nodeId, 0);
    ASSERT_EQ(actorId, 1);
    ASSERT_EQ(status, 1);
  }

  TEST_F(DebuggerTest, UpdateNodeStatusInvalid)
  {
    debugger.init();
    std::unique_ptr<DebugManager> dm(DebugManager::createInstance());
    dm->selectActor(1);
    ASSERT_TRUE(debugger.tick());

    debugger.updateNodeStatus(0, 2, 1);

    unsigned int nodeId = 1337;
    unsigned int actorId = 1337;
    unsigned int status = 1337;
    ASSERT_FALSE(dm->getNodeUpdates(nodeId, actorId, status));

    ASSERT_EQ(nodeId, 1337);
    ASSERT_EQ(actorId, 1337);
    ASSERT_EQ(status, 1337);
  }

  TEST_F(DebuggerTest, UpdateDebugBlackboardCreate)
  {
    debugger.init();
    std::unique_ptr<DebugManager> dm(DebugManager::createInstance());
    dm->selectActor(1);
    ASSERT_TRUE(debugger.tick());
    debugger.updateDebugBlackboard(1, std::pair("new_key", "new_value"));

    ASSERT_TRUE(checkBlackboardContains("new_key")) << "Blackboard doesn't contain created key";
    ASSERT_TRUE(checkBlackboardValue("new_key", "new_value")) << "Blackboard has wrong key value pair";

  }

  TEST_F(DebuggerTest, UpdateDebugBlackboardUpdate)
  {
    debugger.init();
    std::unique_ptr<DebugManager> dm(DebugManager::createInstance());
    dm->selectActor(1);
    ASSERT_TRUE(debugger.tick());
    debugger.updateDebugBlackboard(1, std::pair("new_key", "new_value"));

    ASSERT_TRUE(checkBlackboardContains("new_key")) << "Blackboard doesn't contain created key";
    ASSERT_TRUE(checkBlackboardValue("new_key", "new_value")) << "Blackboard has wrong key value pair";

    debugger.updateDebugBlackboard(1, std::pair("new_key", "update_value"));
    
    ASSERT_TRUE(checkBlackboardContains("new_key")) << "Blackboard doesn't contain created key";
    ASSERT_TRUE(checkBlackboardValue("new_key", "update_value")) << "Blackboard has wrong key value pair";
  }

  TEST_F(DebuggerTest, UpdateDebugBlackboardDelete)
  {
    debugger.init();
    std::unique_ptr<DebugManager> dm(DebugManager::createInstance());
    dm->selectActor(1);
    ASSERT_TRUE(debugger.tick());
    debugger.updateDebugBlackboard(1, std::pair("new_key", "new_value"));

    ASSERT_TRUE(checkBlackboardContains("new_key")) << "Blackboard doesn't contain created key";
    ASSERT_TRUE(checkBlackboardValue("new_key", "new_value")) << "Blackboard has wrong key value pair";

    debugger.updateDebugBlackboard(1, std::pair("new_key", ""));

    ASSERT_FALSE(checkBlackboardContains("new_key")) << "Blackboard contains deleted key";
  }

  TEST_F(DebuggerTest, UpdateDebugBlackboardDeleteNotExist)
  {
    debugger.init();
    std::unique_ptr<DebugManager> dm(DebugManager::createInstance());
    dm->selectActor(1);
    ASSERT_TRUE(debugger.tick());
    debugger.updateDebugBlackboard(1, std::pair("new_key", "new_value"));

    ASSERT_TRUE(checkBlackboardContains("new_key")) << "Blackboard doesn't contain created key";
    ASSERT_TRUE(checkBlackboardValue("new_key", "new_value")) << "Blackboard has wrong key value pair";

    debugger.updateDebugBlackboard(1, std::pair("false_key", ""));

    ASSERT_TRUE(checkBlackboardContains("new_key")) << "Blackboard doesn't contain created key";
    ASSERT_FALSE(checkBlackboardContains("false_key")) << "Blackboard contains non-existing key";
  }

  TEST_F(DebuggerTest, CreateDebugActorValid)
  {
    debugger.init();
    auto path = std::filesystem::absolute("../../../test/assets/testDesign.json").string();
    debugger.createDebugActor(0, path);
    debugger.createDebugActor(1, path);

    ASSERT_TRUE(getActors()->contains(0));
    ASSERT_TRUE(getActors()->contains(1));

    ASSERT_STREQ(path.c_str(), getActors()->at(0).c_str());
    ASSERT_STREQ(path.c_str(), getActors()->at(1).c_str());
  }

  TEST_F(DebuggerTest, CreateDebugActorDup)
  {
    debugger.init();
    auto path = std::filesystem::absolute("../../../test/assets/testDesign.json").string();
    debugger.createDebugActor(0, path);
    debugger.createDebugActor(0, path + "&more");

    ASSERT_TRUE(getActors()->contains(0));

    ASSERT_STREQ(path.c_str(), getActors()->at(0).c_str());
  }

  TEST_F(DebuggerTest, ResetDebugBlackboardValid)
  {
    debugger.init();
    std::unique_ptr<DebugManager> dm(DebugManager::createInstance());
    dm->selectActor(1);
    ASSERT_TRUE(debugger.tick());
    debugger.updateDebugBlackboard(1, std::pair("new_key1", "new_value1"));
    debugger.updateDebugBlackboard(1, std::pair("new_key2", "new_value2"));
    debugger.resetDebugBlackboard({{"reset_key", "reset_value"}});

    ASSERT_FALSE(checkBlackboardContains("new_key1")) << "Blackboard contains key after reset";
    ASSERT_FALSE(checkBlackboardContains("new_key2")) << "Blackboard contains key after reset";

    ASSERT_EQ(getBlackboard()->size(), 1);

    ASSERT_TRUE(checkBlackboardContains("reset_key")) << "Blackboard doesn't contain reset key";
    ASSERT_TRUE(checkBlackboardValue("reset_key", "reset_value")) << "Blackboard has wrong key value pair after reset";
  }

  TEST_F(DebuggerTest, ResetDebugBlackboardEmpty)
  {
    debugger.init();
    std::unique_ptr<DebugManager> dm(DebugManager::createInstance());
    dm->selectActor(1);
    ASSERT_TRUE(debugger.tick());

    ASSERT_EQ(getBlackboard()->size(), 0);

    debugger.resetDebugBlackboard({ {"reset_key", "reset_value"} });

    ASSERT_EQ(getBlackboard()->size(), 1);

    ASSERT_TRUE(checkBlackboardContains("reset_key")) << "Blackboard doesn't contain reset key";
    ASSERT_TRUE(checkBlackboardValue("reset_key", "reset_value")) << "Blackboard has wrong key value pair after reset";
  }
}

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