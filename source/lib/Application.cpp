#include "Application.h"

namespace ArborMaster
{
  Application::Application()
  {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    ImGui::CreateContext();

    m_window = glfwCreateWindow(1280, 720, "ArborMaster", nullptr, nullptr);
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    // Initialize GLAD. Close program if fails.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      glfwTerminate();
      exit(0);
    }


    // Enable transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Sets the color of the 'clear' command. This is a dark grey
    glClearColor(0.1f, 0.1f, 0.1f, 1);
    glViewport(0, 0, 1280, 720);

    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImNodes::CreateContext();
    ImNodes::GetIO().LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;

    //Default path, change later
    m_importer.setPath("..\\..\\SaplingTactics\\source\\lib\\BehaviourNodes.h");

    //Bind UI Callbacks
    m_ui.exportCallback = std::bind(&Application::exportTree, this);
    m_ui.importCallback = std::bind(&Application::importNodes, this);
    m_ui.saveCallback = std::bind(&Application::saveTree, this);
    m_ui.newTreeCallback = std::bind(&Application::newTree, this);
    m_ui.loadCallback = std::bind(&Application::loadTree, this);
    m_ui.debuggerCallback = std::bind(&Application::launchDebugger, this);
}
Application::~Application() {
  ImGui_ImplGlfw_Shutdown();
  ImGui_ImplOpenGL3_Shutdown();
  ImNodes::DestroyContext();
  ImGui::DestroyContext();
  glfwDestroyWindow(m_window);
  glfwTerminate();
}
void Application::run() {
  while (!glfwWindowShouldClose(m_window)) {
    // Setup Frame
    glClear(GL_COLOR_BUFFER_BIT);
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //Draw
    m_ui.drawTabs(m_editorTree.getPath());
    m_ui.drawNodeList(m_nf);
    
    if (m_debugManager) {
      m_ui.drawBlackboard(m_debugManager->getBlackboard());
    }
    else {
      m_ui.drawBlackboard(m_editorTree);
    }

    //If toolbar returns true, a popup was opened
    m_ui.drawToolbar(m_exporter.getPath(), m_importer.getPath(), m_editorTree.getPath());


    if (m_debugManager) {
      m_ui.drawActorList(m_debugManager->getAllActors());
    }

    m_ui.pollHotKeys();

    m_ui.drawExportPopup(m_exporter.getPath());
    m_ui.drawImportPopup(m_importer.getPath());
    m_ui.drawNewPopup(m_editorTree.getPath());
    m_ui.drawOpenPopup(m_editorTree.getPath());
    m_ui.drawSaveAsPopup(m_editorTree.getPath());
    m_ui.drawAlertPopup();

    if (m_debugManager) {
      unsigned int actorId, nodeId, status = 0;
      if (m_debugManager->getNodeUpdates(nodeId, actorId, status) && actorId == m_debugManager->getCurrentActor()) {
        m_editorTree.updateNodeStatus(nodeId, status);
      }
    }

    //Draw Tree
    m_editorTree.draw(m_nf);

    ImGui::ShowDemoWindow();

    //Display Frame
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(m_window);
  }
}
void Application::importNodes() {
  int initialSize = m_nf.getNodes().size();
  m_importer.importAll(m_nf.getNodes());
  m_ui.triggerAlert(std::format("Imported {} nodes.", std::abs(initialSize - (int) m_nf.getNodes().size())));
}
void Application::exportTree() {
  if (!m_exporter.exportTree(m_editorTree)) {
    ImGui::OpenPopup("Alert");
    m_ui.triggerAlert("Error exporting tree.");
  }
}
void Application::saveTree(){
  m_exporter.saveDesign(m_editorTree, m_editorTree.getNewId() - 1);
}
void Application::newTree() {
  auto pathString = m_editorTree.getPath();
  m_editorTree = EditorTree();
  m_editorTree.setPath(pathString);
}
void Application::loadTree() {
  auto p = m_editorTree.getPath();
  m_editorTree = EditorTree();
  if (!m_exporter.loadDesign(m_editorTree, m_nf, p)) {
    m_ui.triggerAlert("Error loading tree.");
  }
}

void Application::launchDebugger()
{
  m_debugManager.reset(DebugManager::createInstance());

  if (m_debugManager) {
    m_ui.actorClickCallback = std::bind(&Application::loadTreeRuntime, this, std::placeholders::_1);
  }
  else {
    m_ui.triggerAlert("Could not launch debugger.");
  }
}

bool Application::loadTreeRuntime(unsigned int id) {
  if (m_debugManager->selectActor(id)) {
    auto m = m_debugManager->getAllActors();
    auto debugPath = m.at(id);
    m_editorTree = EditorTree();
    if (m_exporter.loadDesign(m_editorTree, m_nf, debugPath)) {
      return true;
    }
    else {
      m_debugManager.reset(nullptr);
      m_editorTree = EditorTree();
      m_ui.triggerAlert("Could not load tree at runtime, missing node definitions.");
      return false;
    }
    
  }
  return false;
}

}