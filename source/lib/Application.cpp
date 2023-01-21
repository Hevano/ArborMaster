#include "Application.h"

namespace ArborMaster
{
Application::Application() {
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

}
Application::~Application() {
  ImGui_ImplGlfw_Shutdown();
  ImGui_ImplOpenGL3_Shutdown();
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
    ImGui::ShowDemoWindow();
    m_ui.draw(*this);

    //Display Frame
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(m_window);
  }
}
void Application::setSourcePath(const std::string& path) {}
void Application::importNodes() {}
void Application::exportTree() {}
void Application::saveTree() {}
void Application::newTree() {}
}