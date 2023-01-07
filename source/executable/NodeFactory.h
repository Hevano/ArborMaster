#pragma once
#include "TreeNode.h"

#include <string>
#include <unordered_map>
namespace ArborMaster
{
  class NodeFactory
  {
  private:
    std::unordered_map<std::string, TreeNode> m_nodes;

  public:
    TreeNode createNode(const std::string& nodeName) const;
  };
};

