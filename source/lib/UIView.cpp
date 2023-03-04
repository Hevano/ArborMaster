#include "UIView.h"

#include "TreeNode.h"
#include "TreeExporter.h"
#include "NodeImporter.h"
#include "NodeFactory.h"
#include "EditorTree.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"



void ArborMaster::UIView::drawExportPopup(std::string& path)
{
    drawPopup(POPUP_NAMES.at(ToolBarActions::Export), "Tree Designs will be available in you're client code.", path, exportCallback, "Export path");
}

void ArborMaster::UIView::drawImportPopup(std::string& path)
{
    drawPopup(POPUP_NAMES.at(ToolBarActions::Import), "Extracts node definitions from source code.", path, importCallback, "Import path");
}

void ArborMaster::UIView::drawNewPopup(std::string& path)
{
    drawPopup(POPUP_NAMES.at(ToolBarActions::New), "Start designing with an empty tree.", path, newTreeCallback, "Save Path");
}

void ArborMaster::UIView::drawOpenPopup(std::string& path)
{
  drawPopup(POPUP_NAMES.at(ToolBarActions::Open), "Open a saved tree design.", path, loadCallback, "Save Path");
}

void ArborMaster::UIView::drawSaveAsPopup(std::string& path)
{
  drawPopup(POPUP_NAMES.at(ToolBarActions::SaveAs), "Save design as a new file.", path, saveCallback, "Save Path");
}


void ArborMaster::UIView::drawPopup(
  const std::string& name,
  const std::string& msg,
  std::string& path,
  std::function<void(void)> callback,
  std::string fieldText)
{
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  if (ImGui::BeginPopupModal(name.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    ImGui::Text(std::format("{}\n\n", msg).c_str());
    ImGui::Separator();
    ImGui::SetItemDefaultFocus();
    if (!fieldText.empty()) {
      std::string p = path;
      ImGui::InputText(fieldText.c_str(), &p);

      path = p;
    }

    if (ImGui::Button("OK", ImVec2(120, 0))) {
      if (callback) callback();
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0))) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}

bool ArborMaster::UIView::drawToolbar(
  std::string& exportPath, 
  std::string& importPath, 
  std::string& savePath
)
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
    if (toolbarAction != ToolBarActions::None && POPUP_NAMES.contains(toolbarAction)) {
      ImGui::OpenPopup(POPUP_NAMES.at(toolbarAction).c_str());
    }

    return toolbarAction != ToolBarActions::None;
}

void ArborMaster::UIView::drawTabs()
{
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

void ArborMaster::UIView::drawNodeList(const NodeFactory& nodeCache)
{
    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoResize;
    bool nodeListOpen = false;
    const auto viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x * 0.15f, viewport->WorkSize.y * 0.8f - 30), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + viewport->WorkSize.x * 0.85f, viewport->WorkPos.y * 0.8f + 30), ImGuiCond_Always, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Node List", nullptr, windowFlags);
    for (auto& [name, node] : nodeCache.getNodes()) {
        drawNode(node, true);
    }
    ImGui::End();
}

void ArborMaster::UIView::drawNode(const TreeNode& n, bool draggable)
{
    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoResize;
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild(n.name.c_str(), ImVec2(200, 100), true, windowFlags);
    // Our buttons are both drag sources and drag targets here!
    if (draggable && ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
        // Set payload to carry the index of our item (could be anything)
        ImGui::SetDragDropPayload("NODE_DROP", &n.name, sizeof(std::string));
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
    ImGui::TextWrapped(std::string("Max children: " + std::to_string(n.childCap)).c_str());
    ImGui::EndChild();
    ImGui::PopStyleVar();
}

//TODO
void ArborMaster::UIView::drawBlackboard(const EditorTree& tree)
{
    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoResize;
    bool nodeListOpen = false;
    const auto viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, viewport->WorkSize.y * 0.2f), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y + viewport->WorkSize.y * 0.8f), ImGuiCond_Always, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Blackboard", nullptr, windowFlags);
    float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
    ImVec2 itemSize(200, 20);

    std::vector<std::string> keys;
    keys.reserve(tree.getBlackboard().data.size());
    for (auto& k : tree.getBlackboard().data) {
        keys.push_back(k);
    }

    auto& style = ImGui::GetStyle();
    for (int n = 0; n < keys.size(); n++) {
        ImGui::PushID(n);
        ImGui::BulletText(std::format("key: {}", keys[n]).c_str());
        float last_button_x2 = ImGui::GetItemRectMax().x;
        float next_button_x2 = last_button_x2 + style.ItemSpacing.x + itemSize.x;  // Expected position if next button was on same line
        if (n + 1 < keys.size() && next_button_x2 < window_visible_x2)
            ImGui::SameLine();
        ImGui::PopID();
    }
    ImGui::End();
}
