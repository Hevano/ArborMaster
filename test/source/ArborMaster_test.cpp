#include <gtest/gtest.h>
#include "lib.hpp"
#include "EditorTreeTest.h"
#include "AdapterTest.h"
#include "NodeImporterTest.h"
#include "TreeExporterTest.h"
#include "DebugManagerTest.h"
#include "DebuggerTest.h"

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}