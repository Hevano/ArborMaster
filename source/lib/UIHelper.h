#pragma once

#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"



namespace ArborMaster
{

// Forward declarations
class Application;
class BehaviourTree;

class UIHelper
{
private:
  ImVec2 m_screenOffset;
public:
  void draw(const Application& a);

private:
  void drawExportPopup(const Application& a);
  void drawImportPopup(const Application& a);
  void drawToolbar(const Application& a);
  void drawTree(const Application& a, const BehaviourTree& bt);
};

}
