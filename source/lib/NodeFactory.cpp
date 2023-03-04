#include "NodeFactory.h"

namespace ArborMaster
{
TreeNode NodeFactory::createNode(const std::string& nodeName) const
{
  return TreeNode();
}
const std::unordered_map<std::string, TreeNode>& NodeFactory::getNodes() const
{
  return m_nodes;
}
}