#pragma once

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <optional>
#include <set>

#include "vwindow.hpp"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif 

const std::vector<const char*> validationLayers = {
  "VK_LAYER_KHRONOS_validation"  
};

const std::vector<const char*> deviceExtensions = {
  VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct QueueFamilyIndices
{
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  bool isComplete(){
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
};

class VDevice : public VWindow 
{
  public: 
    VkInstance instance;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice logicalDevice = VK_NULL_HANDLE;
    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkQueue presentQueue = VK_NULL_HANDLE;
    QueueFamilyIndices queueFamilyIndices;
    VkSurfaceKHR surface;

  protected:
    VDevice();
    
    VkResult vkResult(VkResult, std::string);

    VkResult createInstance();
    
    VkResult pickPhysicalDevice();
    bool isDeviceSuitable(VkPhysicalDevice);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice);

    VkResult createLogicalDevice();

    VkResult createSurface();

    void setupDebugMessenger();
    VkDebugUtilsMessengerEXT debugMessenger;
    void DestroyDebugUtilsMessengerEXT(VkInstance, VkDebugUtilsMessengerEXT, const VkAllocationCallbacks*);

  private:    
    VkResult CreateDebugUtilsMessengerEXT(VkInstance, const VkDebugUtilsMessengerCreateInfoEXT*, const VkAllocationCallbacks*, VkDebugUtilsMessengerEXT*);
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT&);
    std::vector<const char*> getRequiredExtensions();
    bool checkValidationLayerSupport();
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }

};

