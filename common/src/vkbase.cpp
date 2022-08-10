#include "vkbase.hpp"

VkBase::VkBase()
{

}

void VkBase::run()
{
  initGLFWwindow();
  mainLoop();
  cleanupGLFWwindow();
}

void VkBase::mainLoop()
{
  while (!glfwWindowShouldClose(glfwWindow))
  {
    processInput(glfwWindow);
    glfwPollEvents();
  }
}