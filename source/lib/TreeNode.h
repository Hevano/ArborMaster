#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>
namespace ArborMaster
{
class TreeNode
{
public:
  std::string name;
  //ImGui::vec2 screenPosition
  std::vector<std::shared_ptr<TreeNode>> children;
  int childCap;
  std::unordered_set<std::string> blackboardKeys;

public:
  //Moves child at oldIndex to newIndex, pushing up all children between old to new index by one
  void reorderChild(int oldIndex, int newIndex);
  //Copies child into children at index, up to childCap. Returns false on invalid index or above cap
  bool insertChild(const TreeNode& t, int index = 0);
  void removeChild(const int index);
};
}

