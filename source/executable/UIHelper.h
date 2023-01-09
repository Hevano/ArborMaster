#pragma once

//Forward declarations
class Application;
class BehaviourTree;

namespace ArborMaster
{

class UIHelper
{
private:
  // ImGui::vec2 m_screenOffset;
public:
  void draw(Application a);

private:
  void drawExportPopup(Application a);
  void drawImportPopup(Application a);
  void drawToolbar(Application a);
  void drawTree(Application a, BehaviourTree bt);
};

}
