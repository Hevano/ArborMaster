#pragma once

#include <gtest/gtest.h>
#include "lib.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace ArborMaster;

//class TreeExporterTest : public ::testing::Test
//{
//protected:
//  TreeExporter exporter;
//  BehaviourTree bt;
//  void SetUp() override
//  { 
//	  exporter.setPath("./assets/");
//    TreeNode child1, child2, child3;
//    child1.name = "child1";
//    child1.blackboardKeys.emplace("key1");
//    child1.childCap = 1;
//    child2.name = "child2";
//    child2.blackboardKeys.emplace("key2");
//    child2.childCap = 2;
//    child3.name = "child3";
//    child3.blackboardKeys.emplace("key3");
//    child3.childCap = 3;
//    bt.getRoot().name = "child0";
//    bt.getRoot().childCap = 3;
//    bt.getRoot().blackboardKeys.emplace("key0");
//    bt.getRoot().insertChild(child1, 0);
//    bt.getRoot().insertChild(child2, 1);
//    bt.getRoot().insertChild(child3, 2);
//  }
//
//  void TearDown() override {}
//};
//
//TEST_F(TreeExporterTest, GetAndSet)
//{
//  exporter.setPath("testVal");
//  EXPECT_EQ(exporter.getPath(), "testval");
//}
//
//TEST_F(TreeExporterTest, Saving) {
//  exporter.saveDesign(bt, "./assets/TreeExporterTest-Saving.json");
//  ASSERT_TRUE(std::filesystem::exists("./assets/TreeExporterTest-Saving.json"));
//  std::ifstream fileStream("./assets/TreeExporterTest-Saving.json");
//  std::ifstream testStream("./assets/TreeExporterTest-Saving-Sample.json"); //TODO: Add expected save output here, or switch to another file
//
//  std::string fileLine, testLine;
//  while (std::getline(fileStream, fileLine) && std::getline(testStream, testLine)){
//    EXPECT_EQ(fileLine, testLine);
//  }
//  EXPECT_EQ(fileStream.tellg(), testStream.tellg());
//  fileStream.close();
//  testStream.close();
//}
//
//TEST_F(TreeExporterTest, Loading)
//{
//  BehaviourTree loadedTree = exporter.loadDesign("./assets/TreeExporterTest-Saving-Sample.json");
//
//  std::unordered_set<std::string> assumedBB = {"key1", "key2", "key3", "key0"};
//  for (const auto& key : assumedBB) {
//    EXPECT_TRUE(loadedTree.getBlackboard().data.contains(key));
//  }
//
//  ASSERT_EQ(loadedTree.getRoot().children.size(), 3);
//
//  EXPECT_EQ(loadedTree.getRoot().name, "child0");
//  EXPECT_EQ(loadedTree.getRoot().children[0]->name, "child1");
//  EXPECT_EQ(loadedTree.getRoot().children[1]->name, "child2");
//  EXPECT_EQ(loadedTree.getRoot().children[2]->name, "child3");
//
//  EXPECT_EQ(loadedTree.getRoot().children[0]->childCap, 1);
//  EXPECT_EQ(loadedTree.getRoot().children[1]->childCap, 2);
//  EXPECT_EQ(loadedTree.getRoot().children[2]->childCap, 3);
//}
//
//TEST_F(TreeExporterTest, SaveAndLoad)
//{
//  exporter.setPath("testVal");
//  EXPECT_EQ(exporter.getPath(), "testval");
//}
//
//TEST_F(TreeExporterTest, Export)
//{
//  FAIL();
//}