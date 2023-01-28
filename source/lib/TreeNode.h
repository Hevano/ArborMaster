#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

namespace ArborMaster
{
class TreeNode
{
public:
  std::string name;
  ImVec2 position;
  std::vector<std::shared_ptr<TreeNode>> children;
  int childCap;
  int id;
  std::unordered_set<std::string> blackboardKeys;

public:
  //Moves child at oldIndex to newIndex, pushing up all children between old to new index by one
  void reorderChild(int oldIndex, int newIndex);
  //Copies child into children at index, up to childCap. Returns false on invalid index or above cap
  bool insertChild(const TreeNode& t, int index = 0);
  void removeChild(const int index);
};
}

