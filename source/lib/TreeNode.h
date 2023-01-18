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
  void reorderChild(const TreeNode& t, int index = 0);
  //Copies child into children at index, up to childCap. Returns false on invalid index or above cap
  bool insertChild(const TreeNode& t, int index = 0);
  void removeChild(const int index);
};
}

