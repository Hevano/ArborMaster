#include "UIHelper.h"
#include "Application.h"
#include "BehaviourTree.h"
#include "TreeNode.h"
#include "EditorNode.h"
#include "misc/cpp/imgui_stdlib.h"
#include "imgui_internal.h"
#include "imnodes_internal.h"


#include <format>
#include <utility>
#include <unordered_set>


namespace ArborMaster
{
void UIHelper::draw(Application& a)
{

  drawEditorTree(a);
  drawToolbar(a);
  drawTabs(a);
  drawNodeList(a);
  drawBlackboard(a);
}
void UIHelper::drawExportPopup(Application& a)
{
  // Always center this window when appearing
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  if (ImGui::BeginPopupModal("Tree Export", NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    ImGui::Text("Tree Designs will be available in you're client code.\n\n");
    ImGui::Separator();
    ImGui::SetItemDefaultFocus();
    std::string path = a.m_exporter.getPath();
    ImGui::InputText("Export path", &path);

    a.m_exporter.setPath(path);

    if (ImGui::Button("OK", ImVec2(120, 0))) {
      a.m_exporter.exportTree(m_adjList, m_editorNodes);
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
void UIHelper::drawToolbar(Application& a)
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
    ImGui::SetDragDropPayload("DND_DEMO_CELL", &n.name, sizeof(std::string));
    drawNode(n, false);
    ImGui::EndDragDropSource();
  }
  ImGui::TextWrapped(n.name.c_str());
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
void UIHelper::drawNodeList(Application& a) {
  ImGuiWindowFlags windowFlags = 0;
  windowFlags |= ImGuiWindowFlags_NoMove;
  windowFlags |= ImGuiWindowFlags_NoResize;
  bool nodeListOpen = false;
  const auto viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x * 0.15f, viewport->WorkSize.y * 0.8f - 30), ImGuiCond_Always);
  ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + viewport->WorkSize.x * 0.85f , viewport->WorkPos.y * 0.8f + 30), ImGuiCond_Always, ImVec2(0.0f, 0.0f));
  ImGui::Begin("Node List", nullptr, windowFlags);
  for (auto& [name, node] : a.m_nf.getNodes()) {
    drawNode(node, true);
  }
  
  ImGui::End();
}

void UIHelper::loadEditorTree(Application& a) {
  /*for (const auto& [name, node] : a.m_nf.getNodes()) {
    m_editorNodes.emplace(++m_editorId, EditorNode(node, ImVec2(), m_editorId));
  }*/
}

void UIHelper::drawEditorTree(Application& a)
{
  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration
      | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings
      | ImGuiWindowFlags_NoBringToFrontOnFocus;
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->WorkPos);
  ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize);
  ImGui::Begin("Node Editor", nullptr, windowFlags);
  ImNodes::BeginNodeEditor();
  if (ImGui::GetCurrentContext()->DragDropActive) {
    ImGui::InvisibleButton("##canvas", ImGui::GetWindowSize());
    if (ImGui::BeginDragDropTarget()) {
      if (const ImGuiPayload* payload =
              ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
      {
        IM_ASSERT(payload->DataSize == sizeof(std::string));
        std::string droppedTree = *(const std::string*)payload->Data;
        int id = ++m_editorId;
        m_editorNodes.emplace(id, EditorNode(a.m_nf.getNodes()[droppedTree], ImGui::GetMousePos(), id));
        m_freeNodes.emplace(id);
        ImNodes::SetNodeScreenSpacePos(id, ImGui::GetMousePos());
        ImNodes::SnapNodeToGrid(id);
      }
      ImGui::EndDragDropTarget();
    }
  }

  ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(255, 0, 0, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(255, 20, 20, 255));

  //Draw Root Node
  ImNodes::BeginNode(1);

  ImNodes::BeginNodeTitleBar();
  ImGui::TextUnformatted("root");
  ImNodes::EndNodeTitleBar();

  ImNodes::BeginOutputAttribute(1 << 16);
  ImNodes::EndOutputAttribute();
  ImNodes::EndNode();

  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();

  // Draw nodes connected to the tree
  for (auto& [parentId, childList] : m_adjList) {
    int childCount = 1;

    std::sort(childList.begin(), childList.end(), [this](int i, int j) {
      return m_editorNodes.at(i).position.y < m_editorNodes.at(j).position.y;
    });

    for (auto& childId : childList) {
      ImNodes::BeginNode(childId);

      ImNodes::BeginNodeTitleBar();
      std::string title = "(" + std::to_string(childCount++) + ") " + m_editorNodes.at(childId).treeNode->name;
      ImGui::TextUnformatted(title.c_str());
      ImNodes::EndNodeTitleBar();

      ImNodes::BeginInputAttribute(childId << 8);
      ImNodes::EndInputAttribute();

      ImNodes::BeginOutputAttribute(childId << 16);
      if(m_adjList[childId].size() > m_editorNodes.at(childId).treeNode->childCap) 
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
      ImGui::Text(std::format("children {}/{}", m_adjList[childId].size(), m_editorNodes.at(childId).treeNode->childCap).c_str());
      if(m_adjList[childId].size() > m_editorNodes.at(childId).treeNode->childCap) 
        ImGui::PopStyleColor();
      ImNodes::EndOutputAttribute();
      ImNodes::EndNode();

      m_editorNodes.at(childId).position = ImNodes::GetNodeGridSpacePos(childId);
    }
  }
  
  //Draw remaining nodes
  for (auto id : m_freeNodes) {
    auto editorNode = m_editorNodes[id];
    ImNodes::BeginNode(id);

    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted(editorNode.treeNode->name.c_str());
    ImNodes::EndNodeTitleBar();

    ImNodes::BeginInputAttribute(id << 8);
    ImNodes::EndInputAttribute();

    ImNodes::BeginOutputAttribute(id << 16);
    ImGui::Text("children");
    ImNodes::EndOutputAttribute();
    ImNodes::EndNode();
  }

  //Draw all links
  for (const auto& [id, editorLink] : m_editorLinks) {
    ImNodes::Link(id, editorLink.startId, editorLink.endId);
  }

  ImNodes::EndNodeEditor();
  ImGui::End();

  //Add new links
  EditorLink link;
  if (ImNodes::IsLinkCreated(&link.startId, &link.endId)) {
    int parentId = link.startId >> 16;
    int childId = link.endId >> 8;

    if (!m_freeNodes.contains(childId)) {
      auto oldLinkIt = m_editorLinks.end();
      for (auto existingLinkIt = m_editorLinks.begin(); existingLinkIt != m_editorLinks.end(); existingLinkIt++) {
        if (existingLinkIt->second.endId >> 8 == childId) {
          oldLinkIt = existingLinkIt;
        }
      }
      if (oldLinkIt != m_editorLinks.end()) {
        deleteLink(oldLinkIt->second);
      }
    }
    m_adjList[parentId].push_back(childId);
    link.id = ++m_editorId;
    m_editorLinks[link.id] = link;
    m_freeNodes.erase(childId);
  }

  //Remove existing links
  int link_id;
  if (ImNodes::IsLinkDestroyed(&link_id)) {
    if (m_editorLinks.count(link_id) != 1) {
      throw std::exception("Tried to remove link not in map");
    }
    deleteLink(m_editorLinks[link_id]);
  }

  // Delete nodes
  int numSelected = ImNodes::NumSelectedNodes();
  if (numSelected != 0 && ImGui::IsKeyPressed(ImGuiKey_Delete, false)) {
    int *selectedIds = new int(numSelected);
    ImNodes::GetSelectedNodes(selectedIds);
    if (selectedIds != nullptr) {
      for (int i = 0; i < numSelected; i++) {
        deleteEditorNode(m_editorNodes[selectedIds[i]]);
      }
    }
  }

}
void UIHelper::deleteLink(EditorLink link) {
  int parentId = link.startId >> 16;
  int childId = link.endId >> 8;
  for (auto& [p, childList] : m_adjList) {
    if (p == parentId) {
      auto it = std::find(childList.begin(), childList.end(), childId);
      if (it != childList.end()) {
        m_freeNodes.emplace(childId);
        childList.erase(it);
      }
    }
  }
  m_editorLinks.erase(link.id);
}

void UIHelper::deleteEditorNode(EditorNode node) {
  if (m_freeNodes.contains(node.id)) {
    m_freeNodes.erase(node.id);
  } else {
    int parentId = -1;
    int linkId = -1;
    for (auto& [id, link] : m_editorLinks) {
      if (link.endId >> 8 == node.id) {
        linkId = link.id;
        parentId = link.startId >> 16;
      }
    }
    if (parentId != -1 && linkId != -1) {
      auto it = std::find(m_adjList[parentId].begin(), m_adjList[parentId].end(), node.id);
      m_adjList[parentId].erase(it);
      m_editorLinks.erase(linkId);
    }
  }
  m_editorNodes.erase(node.id);
}

}


