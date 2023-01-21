#include "UIHelper.h"
#include "Application.h"
#include "BehaviourTree.h"
#include "TreeNode.h"

namespace ArborMaster
{
void UIHelper::draw(const Application& a) {
  drawToolbar(a);
  drawTabs(a);
  drawNodeList(a);
  drawBlackboard(a);
}
void UIHelper::adjustTreeLayout(const TreeNode& root) {}
void UIHelper::drawExportPopup(const Application& a) {}
void UIHelper::drawImportPopup(const Application& a) {}
void UIHelper::drawToolbar(const Application& a) {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New", "CTRL+N")) {
      }
      if (ImGui::MenuItem("Open", "CTRL+O")) {
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Save", "CTRL+S")) {
      }
      if (ImGui::MenuItem("Save As", "CTRL+SHIFT+S")) {
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
  ImGui::TextWrapped(
      "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
      "eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
  ImGui::End();
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
  ImGui::TextWrapped(
      "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
      "eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
  ImGui::End();
}
void UIHelper::drawTree(const Application& a, const BehaviourTree& bt) {}
}


