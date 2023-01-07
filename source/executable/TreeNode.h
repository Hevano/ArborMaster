#pragma once
#include <string>
#include <vector>
#include <memory>
#include <set>
namespace ArborMaster
{
class TreeNode
{
public:
  std::string name;
  //ImGui::vec2 screenPosition
  std::vector<std::shared_ptr<TreeNode>> children;
  int childCap;
  std::set<std::string> blackboardKeys;

public:
  void reorderChild(const TreeNode& t, int index);
  void addChild(TreeNode, int index);
};
}

