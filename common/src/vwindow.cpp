#include "vwindow.hpp"

VWindow::VWindow(){}

void VWindow::initGLFWwindow()
{
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  glfwWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ESEL", nullptr, nullptr);
}

void VWindow::processInput(GLFWwindow* window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
    glfwSetWindowShouldClose(window, true);
  }
    
}

void VWindow::cleanupGLFWwindow()
{
  glfwDestroyWindow(glfwWindow);
  glfwTerminate();
}