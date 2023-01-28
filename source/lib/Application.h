#pragma once
#include "BehaviourTree.h"
#include "AIActor.h"
#include "NodeFactory.h"
#include "UIHelper.h"


#include <unordered_map>
#include <string>
#include "NodeImporter.h"
#include "TreeExporter.h"

#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include "imnodes.h"

namespace ArborMaster
{
  class Application
  {
    friend class UIHelper;
  private:
    BehaviourTree m_editorTree;
    std::unordered_map<std::string, AIActor> m_actors;
    NodeFactory m_nf;
    NodeImporter m_importer;
    TreeExporter m_exporter;
    UIHelper m_ui;

    struct GLFWwindow* m_window;


  public:
    Application();
    ~Application();
    void run();
    void setSourcePath(const std::string& path);
    void importNodes();
    void exportTree();
    void saveTree();
    void newTree();
  };
}
