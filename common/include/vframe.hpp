#pragma once

#include "imgui.h"
#include "imgui_impl_vulkan.h"
#include <string>
#include <iostream>
#include <vector>



class VFrame
{
  public:
    VFrame();
    
    void initFrame(VkDevice*, VkQueue*);
    VkDevice* logicalDevice;
    VkQueue* graphicsQueue;

    ImTextureID frameTexture;
    bool showFrame = false;
    int width, height;
    unsigned char* pixels = nullptr;    

    bool createFrameTexture(VkCommandBuffer);
    void destroyFrameObjects();
    void destroyFrameViewObjects();
    void renderLoop(ImGui_ImplVulkanH_Window*, std::vector<u_char>*, int*, int*);
    VkResult vkResult(VkResult, std::string);
};