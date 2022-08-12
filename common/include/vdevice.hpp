#pragma once

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <optional>

#include "vwindow.hpp"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif 

const std::vector<const char*> validationLayers = {
  "VK_LAYER_KHRONOS_validation"  
};

struct QueueFamilyIndices
{
  std::optional<uint32_t> graphicsFamily;

  bool isComplete(){
    return graphicsFamily.has_value();
  }
};

class VDevice : public VWindow 
{
  public: 
    VkInstance instance;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice logicalDevice = VK_NULL_HANDLE;
    VkQueue graphicsQueue = VK_NULL_HANDLE;
    QueueFamilyIndices queueFamilyIndices;

  protected:
    VDevice();
    VkResult createInstance();
    VkResult vkResult(VkResult, std::string);
    
    VkResult pickPhysicalDevice();
    bool isDeviceSuitable(VkPhysicalDevice);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice);

    VkResult createLogicalDevice();

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

