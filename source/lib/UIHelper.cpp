#include "UIHelper.h"
#include "Application.h"
#include "BehaviourTree.h"
#include "TreeNode.h"

namespace ArborMaster
{
void UIHelper::draw(const Application& a) {
  drawToolbar(a);
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
void UIHelper::drawTabs(const Application& a) {}
void UIHelper::drawTree(const Application& a, const BehaviourTree& bt) {}
}


