#pragma once
#include "NodeFactory.h"
#include "UIView.h"
#include "NodeImporter.h"
#include "TreeExporter.h"
#include "EditorTree.h"
#include "DebugManager.h"

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
    NodeFactory m_nf;
    NodeImporter m_importer;
    TreeExporter m_exporter;
    UIView m_ui;
    EditorTree m_editorTree;
    std::vector<EditorNode> m_editorNodes;
    std::unique_ptr<DebugManager> m_debugManager;

    struct GLFWwindow* m_window;

  private:
    bool loadTreeRuntime(unsigned int id);


  public:
    Application();
    ~Application();
    void run();
    void importNodes();
    void exportTree();
    void saveTree();
    void newTree();
    void loadTree();
    void launchDebugger();
  };
}
