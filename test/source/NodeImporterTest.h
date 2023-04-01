#pragma once

#include <gtest/gtest.h>
#include "lib.hpp"

using namespace ArborMaster;

//class NodeImporterTest : public ::testing::Test
//{
//protected:
//  NodeImporter importer;
//  void SetUp() override
//  { 
//	  importer.setPath("./assets");
//  }
//
//  void TearDown() override {}
//}; 
//
//TEST_F(NodeImporterTest, GetAndSet)
//{
//  importer.setPath("testVal");
//  EXPECT_EQ(importer.getPath(), "testval");
//}
//
//TEST_F(NodeImporterTest, ImportNodeFromString)
//{
//	//TODO: Write test once node text schema has been defined
//  FAIL();
//}
//
//TEST_F(NodeImporterTest, ImportNodesFromPath)
//{
//  // TODO: Write test once node text schema has been defined
//  FAIL();
//}