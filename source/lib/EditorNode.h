#pragma once

#include "TreeNode.h"
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

namespace ArborMaster
{
struct EditorNode
{

  enum class NodeStatus : unsigned int {
    RUNNING = 0,
    FAILED = 1,
    SUCCEEDED = 2
  };

  int id;
  ImVec2 position;
  TreeNode const* treeNode;
  NodeStatus staus = NodeStatus::SUCCEEDED;
  EditorNode() = default;

  EditorNode(const TreeNode& tn, ImVec2 pos, int id)
      : treeNode(&tn)
      , position(pos)
      , id(id)
  {
  }
};

struct EditorLink
{
  int id;
  int startId;
  int endId;
};

}  // namespace ArborMaster