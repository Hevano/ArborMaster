#pragma once
#include "AIActor.h"
#include "NodeFactory.h"
#include "UIView.h"
#include "NodeImporter.h"
#include "TreeExporter.h"
#include "EditorTree.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "imnodes.h"

#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/containers/flat_map.hpp>

#include <unordered_map>
#include <string>
#include <vector>

namespace ArborMaster
{
  namespace ipc = boost::interprocess;
  class Application
  {
    friend class UIHelper;
  private:
    
    std::unordered_map<std::string, AIActor> m_actors;
    NodeFactory m_nf;
    NodeImporter m_importer;
    TreeExporter m_exporter;
    UIView m_ui;
    EditorTree m_editorTree;
    std::vector<EditorNode> m_editorNodes;

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
    void loadTree();
  };
}
