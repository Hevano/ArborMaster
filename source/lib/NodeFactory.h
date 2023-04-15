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
    //const and non-const getters
    const std::unordered_map<std::string, TreeNode>& getNodes() const;
    inline std::unordered_map<std::string, TreeNode>& getNodes()
    {
      return const_cast<std::unordered_map<std::string, TreeNode>&>(const_cast<const NodeFactory*>(this)->getNodes());
    }
  };
};

