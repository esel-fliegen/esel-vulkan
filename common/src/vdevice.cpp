#include "vdevice.hpp"

VDevice::VDevice(){}

void VDevice::createInstance()
{
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "esel-vulkan";
  appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
  appInfo. apiVersion = VK_API_VERSION_1_2;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  createInfo.enabledExtensionCount = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;
  createInfo.enabledLayerCount = 0;

  vkResult(vkCreateInstance(&createInfo, VK_NULL_HANDLE, &instance),
    "create instance");
}

void VDevice::vkResult(VkResult result, const char* msg)
{
  if(result != VK_SUCCESS)  
    throw std::runtime_error("Failed to " + *msg);  
  else
  {
    std::cout<<*msg<<" successful."<<std::endl;
  }

}