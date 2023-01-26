#pragma once

#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include <string>

namespace ArborMaster
{

// Forward declarations
class Application;
class BehaviourTree;
class TreeNode;

class UIHelper
{
private:
  ImVec2 m_screenOffset;
  inline static const std::string EXPORT_POPUP_NAME = "Tree Export";
  inline static const std::string IMPORT_POPUP_NAME = "Node Import";
  inline static const std::string OPEN_POPUP_NAME = "Open Tree Design";
  inline static const std::string NEW_POPUP_NAME = "New Tree Design";
  inline static const std::string SAVE_AS_POPUP_NAME = "Save As";

public:
  void draw(const Application& a);
  void adjustTreeLayout(const TreeNode& root);
private:
  enum class ToolBarActions
  {
	None,
	New,
	Save,
	SaveAs,
	Open,
	Export,
	Import
  };

  void drawExportPopup(const Application& a);
  void drawImportPopup(const Application& a);
  void drawNewPopup(const Application& a);
  void drawSaveAsPopup(const Application& a);
  void drawOpenPopup(const Application& a);

  void drawWorkSurface(const Application& a);
  void drawToolbar(const Application& a);
  void drawTabs(const Application& a);
  void drawBlackboard(const Application& a);

  void drawNode(const TreeNode& n, bool draggable = false);
  void drawNodeList(const Application& a);
  int getSubTreeWidth(const TreeNode& root);
  void drawTree(BehaviourTree& bt);
  void drawTree(TreeNode& n);
};

}
