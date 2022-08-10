#include "vkwindow.hpp"

VkWindow::VkWindow(){}

void VkWindow::initGLFWwindow()
{
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  glfwWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ESEL", nullptr, nullptr);
}

void VkWindow::processInput(GLFWwindow* window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void VkWindow::cleanupGLFWwindow()
{
  glfwDestroyWindow(glfwWindow);
  glfwTerminate();
}