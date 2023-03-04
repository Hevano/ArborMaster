#include "BehaviourTree.h"

namespace ArborMaster
{
BehaviourTree::BehaviourTree() {
  m_root = std::make_shared<TreeNode>(TreeNode());
}
void BehaviourTree::updateBlackboard() {}
TreeNode& BehaviourTree::getRoot()
{
  return *m_root;
}
const Blackboard& BehaviourTree::getBlackboard() const
{
  return m_blackboard;
}
}