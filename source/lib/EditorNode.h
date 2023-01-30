#pragma once

#include "TreeNode.h"
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

namespace ArborMaster
{
struct EditorNode
{
  int id;
  ImVec2 position;
  const TreeNode& treeNode;

  EditorNode(const TreeNode& tn, ImVec2 pos, int id)
      : treeNode(tn)
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