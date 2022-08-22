#include "vbase.hpp"

VBase::VBase()
{
  vFrame.initFrame(&logicalDevice, &graphicsQueue);
}

void VBase::run()
{
  initGLFWwindow();
  initVulkan();
  initImgui();
  mainLoop();  
  cleanup();
}

void VBase::show(std::vector<u_char>* frame,  int* width, int* height)
{
  this->frame = frame;
  this->width = width;
  this->height = height;
  run();
}

void VBase::initVulkan()
{
  createInstance();
  setupDebugMessenger();
  createSurface();
  pickPhysicalDevice();
  createLogicalDevice();
  

}

void VBase::mainLoop()
{
  while (!glfwWindowShouldClose(glfwWindow))
  {
    processInput(glfwWindow);
    glfwPollEvents();
    renderLoopBegin();
    vFrame.renderLoop(&mainWindowData, frame, width, height);
    renderLoopEnd(&mainWindowData);
    vFrame.destroyFrameViewObjects();
  }
}

void VBase::cleanup()
{
  cleanImgui();
  vkDestroyDevice(logicalDevice, VK_NULL_HANDLE);
  if(enableValidationLayers)
  {
    DestroyDebugUtilsMessengerEXT(instance, debugMessenger, VK_NULL_HANDLE);
  }
  vkDestroyInstance(instance, VK_NULL_HANDLE);
  cleanupGLFWwindow();
}

