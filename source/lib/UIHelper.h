#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"


namespace ArborMaster
{

// Forward declarations
class Application;
class BehaviourTree;
class TreeNode;
class EditorNode;
class EditorLink;

static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs)
{
  return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y);
}

class UIHelper
{
private:
  inline static int m_editorId = 2; //root node, first output, and first link, attribute are reserved
  inline static const std::string EXPORT_POPUP_NAME = "Tree Export";
  inline static const std::string IMPORT_POPUP_NAME = "Node Import";
  inline static const std::string OPEN_POPUP_NAME = "Open Tree Design";
  inline static const std::string NEW_POPUP_NAME = "New Tree Design";
  inline static const std::string SAVE_AS_POPUP_NAME = "Save As";

  std::unordered_map<int, EditorNode> m_editorNodes;
  std::unordered_set<int> m_freeNodes;
  std::unordered_map<int, EditorLink> m_editorLinks;
  std::unordered_map<int, std::vector<int>> m_adjList;

public:
  void draw(Application& a);
  void adjustTreeLayout(const TreeNode& root);
  void loadEditorTree(Application& a);

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

  void drawExportPopup(Application& a);
  void drawImportPopup(const Application& a);
  void drawNewPopup(const Application& a);
  void drawSaveAsPopup(const Application& a);
  void drawOpenPopup(const Application& a);

  void drawWorkSurface(const Application& a);
  void drawToolbar(Application& a);
  void drawTabs(const Application& a);
  void drawBlackboard(const Application& a);

  void drawNode(const TreeNode& n, bool draggable = false);
  void drawNodeList(Application& a);
  void drawEditorTree(Application& a);
  void deleteLink(EditorLink link);
  void deleteEditorNode(EditorNode node);
};

}
