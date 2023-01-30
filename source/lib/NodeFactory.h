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
    //TODO: Consider move semantics
    TreeNode createNode(const std::string& nodeName) const;
    std::unordered_map<std::string, TreeNode>& getNodes();
  };
};

