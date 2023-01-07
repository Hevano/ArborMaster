#pragma once
#include "TreeNode.h"

#include <string>
#include <memory>
namespace ArborMaster {
  class BehaviourTree
  {
  private:
    std::shared_ptr<TreeNode> m_root;

  public:
    void setPath(const std::string& path);
    const std::string & getPath() const;
  };
}
