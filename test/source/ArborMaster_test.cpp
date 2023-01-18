#include <gtest/gtest.h>
#include "lib.h"
#include "TreeNodeTest.h"
#include "BehaviourTreeTest.h"




int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}