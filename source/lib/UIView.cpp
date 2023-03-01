#include "UIView.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

void ArborMaster::UIView::drawExportPopup(std::string& path)
{
    // Always center this window when appearing
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal(EXPORT_POPUP_NAME.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Tree Designs will be available in you're client code.\n\n");
        ImGui::Separator();
        ImGui::SetItemDefaultFocus();
        auto p = path;
        ImGui::InputText("Export path",&p);

        path = p;

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            if (exportCallback) {
                exportCallback();
            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void ArborMaster::UIView::drawImportPopup(std::string& path)
{
    // Always center this window when appearing
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal(
        IMPORT_POPUP_NAME.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Extracts node definitions from source code.\n\n");
        ImGui::Separator();
        ImGui::SetItemDefaultFocus();
        std::string p;
        ImGui::InputText("Import path", &p);

        path = p;

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            if (importCallback) {
                importCallback();
            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void ArborMaster::UIView::drawNewPopup(std::string& path)
{
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal(
        .c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
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

void ArborMaster::UIView::drawLoadPopup(std::string& path)
{
}

void ArborMaster::UIView::drawSaveAsPopup(std::string& path)
{
}

void ArborMaster::UIView::drawToolbar()
{
}

void ArborMaster::UIView::drawTabs()
{
}

void ArborMaster::UIView::drawNodeList(NodeCache nodes)
{
}

void ArborMaster::UIView::drawNode(const TreeNode& n, bool draggable)
{
}

void ArborMaster::UIView::drawBlackboard(const EditorTree& tree)
{
}
