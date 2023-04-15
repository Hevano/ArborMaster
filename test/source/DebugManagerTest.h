#pragma once

#include <gtest/gtest.h>
#include "lib.hpp"
#include "adapter/Debugger.h"

namespace ArborMaster {
class DebugManagerTest : public ::testing::Test
{
protected:
  void SetUp() override {};
  void TearDown() override {};
};

TEST_F(DebugManagerTest, createInstanceEmpty)
{
  ASSERT_EQ(DebugManager::createInstance(), nullptr) << "Debug manager created without existing game instance is not null";
}

TEST_F(DebugManagerTest, createInstanceValid)
{
  //Adapter debugger instance
  ArborMasterAdapter::Debugger d;
  ASSERT_TRUE(d.init());

  std::unique_ptr<DebugManager> dm(DebugManager::createInstance());
  ASSERT_NE(dm.get(), nullptr) << "Create instance resulted in a nullptr";
}

TEST_F(DebugManagerTest, createInstanceFileNotFound)
{
  ArborMasterAdapter::Debugger d;
  ASSERT_TRUE(d.init());

  d.createDebugActor(0, "../../../test/assets/FileNotFound");

  std::unique_ptr<DebugManager> dm(DebugManager::createInstance());
  ASSERT_EQ(dm.get(), nullptr) << "Create instance succedded when file path was invalid";
}

TEST_F(DebugManagerTest, getNodeUpdatesEmpty)
{
  ArborMasterAdapter::Debugger d;
  ASSERT_TRUE(d.init());

  std::unique_ptr<DebugManager> dm(DebugManager::createInstance());
  ASSERT_NE(dm.get(), nullptr) << "Create instance resulted in a nullptr";
  
  unsigned int nodeId = 0;
  unsigned int actorId = 0;
  unsigned int status = 0;
  ASSERT_FALSE(dm->getNodeUpdates(nodeId, actorId, status)) << "Get node updates returned true when queue was empty";

  ASSERT_EQ(nodeId, 0) << "Node update parameter changed with no update in queue";
  ASSERT_EQ(actorId, 0) << "Node update parameter changed with no update in queue";
  ASSERT_EQ(status, 0) << "Node update parameter changed with no update in queue";
}

TEST_F(DebugManagerTest, getNodeUpdatesValid)
{
  ArborMasterAdapter::Debugger d;
  ASSERT_TRUE(d.init());

  std::unique_ptr<DebugManager> dm(DebugManager::createInstance());
  ASSERT_NE(dm.get(), nullptr) << "Create instance resulted in a nullptr";

  dm->selectActor(2);
  d.tick();
  d.updateNodeStatus(1, 2, 3);

  unsigned int nodeId = 0;
  unsigned int actorId = 0;
  unsigned int status = 0;
  ASSERT_TRUE(dm->getNodeUpdates(nodeId, actorId, status)) << "Get node updates returned false when queue had an update";

  ASSERT_EQ(nodeId, 1) << "Node update parameter incorrect";
  ASSERT_EQ(actorId, 2) << "Node update parameter incorrect";
  ASSERT_EQ(status, 3) << "Node update parameter incorrect";
}

TEST_F(DebugManagerTest, getAllActorsValid)
{
  ArborMasterAdapter::Debugger d;
  ASSERT_TRUE(d.init());
  auto path = std::filesystem::absolute("../../../test/assets/testDesign.json").string();
  d.createDebugActor(0, path);
  d.createDebugActor(1, path);

  std::unique_ptr<DebugManager> dm(DebugManager::createInstance());
  ASSERT_NE(dm.get(), nullptr) << "Create instance resulted in a nullptr";

  auto actors = dm->getAllActors();

  EXPECT_EQ(actors.size(), 2) << "Incorrect number of actors retrieved";

  EXPECT_TRUE(actors.contains(0)) << "First actor not retreived";
  EXPECT_TRUE(actors.contains(1)) << "Second actor not retreived";

  EXPECT_EQ(actors[0], path) << "Actor has incorrect design path";
  EXPECT_EQ(actors[1], path) << "Actor has incorrect design path";
}

TEST_F(DebugManagerTest, getAllActorsEmpty)
{
  ArborMasterAdapter::Debugger d;
  ASSERT_TRUE(d.init());

  std::unique_ptr<DebugManager> dm(DebugManager::createInstance());
  ASSERT_NE(dm.get(), nullptr) << "Create instance resulted in a nullptr";

  ASSERT_EQ(dm->getAllActors().size(), 0) << "Get all actors did not return empty dict when no actors existed";
}

TEST_F(DebugManagerTest, getBlackboardValid)
{
  //Adapter debugger instance
  ArborMasterAdapter::Debugger d;
  ASSERT_TRUE(d.init());

  std::unique_ptr<DebugManager> dm(DebugManager::createInstance());
  ASSERT_NE(dm.get(), nullptr) << "Create instance resulted in a nullptr";

  dm->selectActor(2);
  d.tick();
  d.updateDebugBlackboard(2, "bb_key1", "bb_value1");
  d.updateDebugBlackboard(2, "bb_key2", "bb_value2");

  auto bb = dm->getBlackboard();
  EXPECT_EQ(bb.size(), 2) << "Blackboard of incorrect size";
  ASSERT_TRUE(bb.contains("bb_key1")) << "Blackboard missing key1";
  ASSERT_TRUE(bb.contains("bb_key2")) << "Blackboard missing key2";
  ASSERT_EQ(bb["bb_key1"], "bb_value1") << "Key 1 has wrong value";
  ASSERT_EQ(bb["bb_key2"], "bb_value2") << "Key 2 has wrong value";

  d.updateDebugBlackboard(2, "bb_key1", "bb_value1+1");
  d.updateDebugBlackboard(2, "bb_key2", "bb_value2+2");

  bb = dm->getBlackboard();
  EXPECT_EQ(bb.size(), 2) << "Blackboard of incorrect size";
  ASSERT_TRUE(bb.contains("bb_key1")) << "Blackboard missing key1";
  ASSERT_TRUE(bb.contains("bb_key2")) << "Blackboard missing key2";
  ASSERT_EQ(bb["bb_key1"], "bb_value1+1") << "Key 1 has wrong value";
  ASSERT_EQ(bb["bb_key2"], "bb_value2+2") << "Key 2 has wrong value";
}

TEST_F(DebugManagerTest, getBlackboardEmpty)
{
  //Adapter debugger instance
  ArborMasterAdapter::Debugger d;
  ASSERT_TRUE(d.init());

  std::unique_ptr<DebugManager> dm(DebugManager::createInstance());
  ASSERT_NE(dm.get(), nullptr) << "Create instance resulted in a nullptr";

  auto bb = dm->getBlackboard();
  EXPECT_EQ(bb.size(), 0) << "Blackboard should be empty";
}

TEST_F(DebugManagerTest, getBlackboardDeleted)
{
  //Adapter debugger instance
  ArborMasterAdapter::Debugger d;
  ASSERT_TRUE(d.init());

  std::unique_ptr<DebugManager> dm(DebugManager::createInstance());
  ASSERT_NE(dm.get(), nullptr) << "Create instance resulted in a nullptr";

  dm->selectActor(2);
  d.tick();
  d.updateDebugBlackboard(2, "bb_key1", "bb_value1");
  d.updateDebugBlackboard(2, "bb_key2", "bb_value2");

  auto bb = dm->getBlackboard();
  EXPECT_EQ(bb.size(), 2) << "Blackboard of incorrect size";
  ASSERT_TRUE(bb.contains("bb_key1")) << "Blackboard missing key1";
  ASSERT_TRUE(bb.contains("bb_key2")) << "Blackboard missing key2";
  ASSERT_EQ(bb["bb_key1"], "bb_value1") << "Key 1 has wrong value";
  ASSERT_EQ(bb["bb_key2"], "bb_value2") << "Key 2 has wrong value";

  d.updateDebugBlackboard(2, "bb_key1", "bb_value1+1");
  d.updateDebugBlackboard(2, "bb_key2", "");

  bb = dm->getBlackboard();
  EXPECT_EQ(bb.size(), 1) << "Blackboard of incorrect size";
  ASSERT_TRUE(bb.contains("bb_key1")) << "Blackboard missing key1";
  ASSERT_FALSE(bb.contains("bb_key2")) << "Blackboard missing key2";
  ASSERT_EQ(bb["bb_key1"], "bb_value1+1") << "Key 1 has wrong value";

}
}