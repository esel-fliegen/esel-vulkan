#include "vbase.hpp"

VBase::VBase()
{}

void VBase::run()
{
  initGLFWwindow();
  initVulkan();
  mainLoop();  
  cleanup();
}

void VBase::initVulkan()
{
  createInstance();
  setupDebugMessenger();
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
  if(enableValidationLayers)
  {
    DestroyDebugUtilsMessengerEXT(instance, debugMessenger, VK_NULL_HANDLE);
  }
  vkDestroyInstance(instance, VK_NULL_HANDLE);
  cleanupGLFWwindow();
}

