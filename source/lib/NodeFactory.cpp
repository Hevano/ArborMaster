#include "NodeFactory.h"

namespace ArborMaster
{
TreeNode NodeFactory::createNode(const std::string& nodeName) const
{
  return TreeNode();
}
std::unordered_map<std::string, TreeNode>& NodeFactory::getNodes()
{
  return m_nodes;
}
}