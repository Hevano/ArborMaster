#pragma once
#include "TreeNode.h"
#include "Blackboard.h"

#include <string>
#include <memory>
namespace ArborMaster {
  class BehaviourTree
  {
  private:
    std::shared_ptr<TreeNode> m_root;
    Blackboard m_blackboard;

  public:
    BehaviourTree();
    void updateBlackboard();
    TreeNode& getRoot();
    const Blackboard& getBlackboard() const;

  };
}
