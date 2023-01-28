#include "UIHelper.h"
#include "Application.h"
#include "BehaviourTree.h"
#include "TreeNode.h"
#include "misc/cpp/imgui_stdlib.h"


#include <format>


namespace ArborMaster
{


void UIHelper::draw(const Application& a) {

  const int hardcoded_node_id = 1;
  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration
      | ImGuiWindowFlags_NoMove
      | ImGuiWindowFlags_NoSavedSettings
      | ImGuiWindowFlags_NoBringToFrontOnFocus;
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->WorkPos);
  ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize);
  ImGui::Begin("Node Editor", nullptr, windowFlags);
  ImNodes::BeginNodeEditor();

  ImNodes::BeginNode(hardcoded_node_id);
  ImNodes::BeginNodeTitleBar();
  ImNodes::EndNodeTitleBar();
  ImGui::TextUnformatted("simple node :)");
  ImGui::Dummy(ImVec2(80.0f, 45.0f));
  ImNodes::EndNode();

  ImNodes::EndNodeEditor();
  ImGui::End();
  
  drawToolbar(a);
  drawTabs(a);
  drawNodeList(a);
  drawBlackboard(a);
}
void UIHelper::drawExportPopup(const Application& a)
{
  // Always center this window when appearing
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  if (ImGui::BeginPopupModal("Tree Export", NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    ImGui::Text("Tree Designs will be available in you're client code.\n\n");
    ImGui::Separator();
    ImGui::SetItemDefaultFocus();
    std::string path;
    ImGui::InputText("Export path", &path);

    if (ImGui::Button("OK", ImVec2(120, 0))) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0))) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}
void UIHelper::drawImportPopup(const Application& a) {
  // Always center this window when appearing
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  if (ImGui::BeginPopupModal(
          "Node Import", NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    ImGui::Text("Extracts node definitions from source code.\n\n");
    ImGui::Separator();
    ImGui::SetItemDefaultFocus();
    std::string path;
    ImGui::InputText("Import path", &path);

    if (ImGui::Button("OK", ImVec2(120, 0))) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0))) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}
void UIHelper::drawNewPopup(const Application& a) {
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  if (ImGui::BeginPopupModal(
          NEW_POPUP_NAME.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    ImGui::Text("Start designing with an empty tree.\n\n");
    ImGui::Separator();
    ImGui::SetItemDefaultFocus();

    if (ImGui::Button("OK", ImVec2(120, 0))) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0))) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}
void UIHelper::drawSaveAsPopup(const Application& a) {
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  if (ImGui::BeginPopupModal(
          SAVE_AS_POPUP_NAME.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    ImGui::Text("Save design at path\n\n");
    ImGui::Separator();
    ImGui::SetItemDefaultFocus();
    std::string path;
    ImGui::InputText("File Path", &path);

    if (ImGui::Button("OK", ImVec2(120, 0))) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0))) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}
void UIHelper::drawOpenPopup(const Application& a) {
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  if (ImGui::BeginPopupModal(
          OPEN_POPUP_NAME.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    ImGui::Text("Open and edit an existing tree design.\n\n");
    ImGui::Separator();
    ImGui::SetItemDefaultFocus();
    std::string path;
    ImGui::InputText("File path", &path);

    if (ImGui::Button("Open", ImVec2(120, 0))) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0))) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}
void UIHelper::drawWorkSurface(const Application& a) {

}
void UIHelper::drawToolbar(const Application& a)
{
  auto toolbarAction = ToolBarActions::None;
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New", "CTRL+N")) {
        toolbarAction = ToolBarActions::New;
      }
      if (ImGui::MenuItem("Open", "CTRL+O")) {
        toolbarAction = ToolBarActions::Open;
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Save", "CTRL+S")) {
        toolbarAction = ToolBarActions::Save;
      }
      if (ImGui::MenuItem("Save As", "CTRL+SHIFT+S")) {
        toolbarAction = ToolBarActions::SaveAs;
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Export", "CTRL+E")) {
        toolbarAction = ToolBarActions::Export;
      }
      if (ImGui::MenuItem("Import", "CTRL+I")) {
        toolbarAction = ToolBarActions::Import;
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit")) {
      if (ImGui::MenuItem("Undo", "CTRL+Z")) {
      }
      if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {
      }  // Disabled item
      ImGui::Separator();
      if (ImGui::MenuItem("Cut", "CTRL+X")) {
      }
      if (ImGui::MenuItem("Copy", "CTRL+C")) {
      }
      if (ImGui::MenuItem("Paste", "CTRL+V")) {
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("View")) {
      if (ImGui::MenuItem("Unknown", "CTRL+???")) {
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
  switch (toolbarAction) {
    case ToolBarActions::Export:
      ImGui::OpenPopup(EXPORT_POPUP_NAME.c_str());
      break;
    case ToolBarActions::Import:
      ImGui::OpenPopup(IMPORT_POPUP_NAME.c_str());
      break;
    case ToolBarActions::Open:
      ImGui::OpenPopup(OPEN_POPUP_NAME.c_str());
      break;
    case ToolBarActions::New:
      ImGui::OpenPopup(NEW_POPUP_NAME.c_str());
      break;
    case ToolBarActions::Save:
      ImGui::OpenPopup(SAVE_AS_POPUP_NAME.c_str());
      break;
    case ToolBarActions::SaveAs:
      ImGui::OpenPopup(SAVE_AS_POPUP_NAME.c_str());
      break;
    default:
      break;
  }
    
  drawExportPopup(a);
  drawImportPopup(a);
  drawNewPopup(a);
  drawOpenPopup(a);
  drawSaveAsPopup(a);
}
void UIHelper::drawTabs(const Application& a) {
  ImGuiWindowFlags windowFlags = 0;
  windowFlags |= ImGuiWindowFlags_NoTitleBar;
  windowFlags |= ImGuiWindowFlags_NoMove;
  windowFlags |= ImGuiWindowFlags_NoCollapse;
  windowFlags |= ImGuiWindowFlags_NoBackground;
  windowFlags |= ImGuiWindowFlags_NoNav;
  windowFlags |= ImGuiWindowFlags_NoScrollbar;
  windowFlags |= ImGuiWindowFlags_NoResize;

  ImGui::SetNextWindowSize(ImVec2(ImGui::GetMainViewport()->WorkSize.x, 20), ImGuiCond_Always);
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->WorkPos, ImGuiCond_Always, ImVec2(0.0f, 0.0f));
  ImGui::Begin("Tabs", nullptr, windowFlags);
  if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {
    if (ImGui::BeginTabItem("Tree1.json")) {
      ImGui::TextWrapped(
          "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
          "eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Tree2.json")) {
      ImGui::TextWrapped(
          "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
          "eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
      ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
  }
  ImGui::End();
}
void UIHelper::drawBlackboard(const Application& a) {
    ImGuiWindowFlags windowFlags = 0;
  windowFlags |= ImGuiWindowFlags_NoMove;
  windowFlags |= ImGuiWindowFlags_NoResize;
  bool nodeListOpen = false;
  const auto viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, viewport->WorkSize.y * 0.2f), ImGuiCond_Always);
  ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y + viewport->WorkSize.y * 0.8f), ImGuiCond_Always, ImVec2(0.0f, 0.0f));
  ImGui::Begin("Blackboard", nullptr, windowFlags);
  int buttons_count = 20;
  float window_visible_x2 =
      ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
  ImVec2 itemSize(200, 20);
  auto& style = ImGui::GetStyle();
  for (int n = 0; n < buttons_count; n++) {
    ImGui::PushID(n);
    ImGui::BulletText(std::format("key: {}", n).c_str(), itemSize);
    float last_button_x2 = ImGui::GetItemRectMax().x;
    float next_button_x2 = last_button_x2 + style.ItemSpacing.x
        + itemSize.x;  // Expected position if next button was on same line
    if (n + 1 < buttons_count && next_button_x2 < window_visible_x2)
      ImGui::SameLine();
    ImGui::PopID();
  }
  ImGui::End();
}
void UIHelper::drawNode(const TreeNode& n, bool draggable) {
  ImGuiWindowFlags windowFlags = 0;
  windowFlags |= ImGuiWindowFlags_NoMove;
  windowFlags |= ImGuiWindowFlags_NoResize;
  ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
  ImGui::BeginChild(n.name.c_str(), ImVec2(200, 100), true, windowFlags);
  // Our buttons are both drag sources and drag targets here!
  if (draggable && ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
    // Set payload to carry the index of our item (could be anything)
    ImGui::SetDragDropPayload("DND_DEMO_CELL", &n, sizeof(TreeNode));
    drawNode(n, false);
    ImGui::EndDragDropSource();
  }
  ImGui::TextWrapped(n.name.c_str());
  std::string s(std::to_string(ImGui::IsItemClicked()));
  ImGui::TextWrapped(s.c_str());
  if (ImGui::IsItemActive()) {
    ImGui::TextWrapped(std::to_string(ImGui::IsMouseDragging(0)).c_str());
  }
  ImGui::Separator();
  ImGui::TextWrapped("Blackboard Data: ");
  for (const auto& key : n.blackboardKeys) {
    ImGui::BulletText(key.c_str());
  }
  ImGui::TextWrapped(
      std::string("Max children: " + std::to_string(n.childCap)).c_str());
  ImGui::EndChild();
  ImGui::PopStyleVar();
}
void UIHelper::drawNodeList(const Application& a) {
  ImGuiWindowFlags windowFlags = 0;
  windowFlags |= ImGuiWindowFlags_NoMove;
  windowFlags |= ImGuiWindowFlags_NoResize;
  bool nodeListOpen = false;
  const auto viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x * 0.15f, viewport->WorkSize.y * 0.8f - 30), ImGuiCond_Always);
  ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + viewport->WorkSize.x * 0.85f , viewport->WorkPos.y * 0.8f + 30), ImGuiCond_Always, ImVec2(0.0f, 0.0f));
  ImGui::Begin("Node List", nullptr, windowFlags);
  TreeNode tn;
  tn.childCap = 3;
  tn.name = "Sample Node";
  tn.blackboardKeys.emplace("key1");
  drawNode(tn, true);
  ImGui::End();
}
int UIHelper::getSubTreeWidth(const TreeNode& root) {
  int width = 0;
  for (auto& child : root.children) {
    width += getSubTreeWidth(*child);
  }
  return width;
}
void UIHelper::drawTree(BehaviourTree& bt) {
  drawTree(bt.getRoot());
}

//Draws each node in the tree and the 
void UIHelper::drawTree(TreeNode& n) {
}

//Sets the position of the nodes to create a evenly spaced tree layout
void UIHelper::adjustTreeLayout(const TreeNode& root) {
  //Assemble all nodes in a single line, with children equally to the left and right
  // 
}
}


