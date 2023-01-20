#include "NodeFactory.h"

namespace ArborMaster
{
TreeNode NodeFactory::createNode(const std::string& nodeName) const
{
  return TreeNode();
}
void NodeFactory::defineNode(const TreeNode& n) {}
}  // namespace ArborMaster