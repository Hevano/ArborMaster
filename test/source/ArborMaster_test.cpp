#include <gtest/gtest.h>
#include "lib.hpp"
#include "TreeNodeTest.h"
#include "EditorTreeTest.h"
#include "AIActorTest.h"
#include "NodeFactoryTest.h"
#include "NodeImporterTest.h"
#include "TreeExporterTest.h"
#include "BlackboardTest.h"

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}