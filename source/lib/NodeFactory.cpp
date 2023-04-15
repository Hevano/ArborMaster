#include "NodeFactory.h"

namespace ArborMaster
{
const std::unordered_map<std::string, TreeNode>& NodeFactory::getNodes() const
{
  return m_nodes;
}
}