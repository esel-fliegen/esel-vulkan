#include "vbase.hpp"

VBase::VBase()
{

}

void VBase::run()
{
  initGLFWwindow();
  mainLoop();
  
}

void VBase::mainLoop()
{
  while (!glfwWindowShouldClose(glfwWindow))
  {
    processInput(glfwWindow);
    glfwPollEvents();
  }
}

void VBase::cleanup()
{
  vkDestroyInstance(instance, VK_NULL_HANDLE);
  cleanupGLFWwindow();
}