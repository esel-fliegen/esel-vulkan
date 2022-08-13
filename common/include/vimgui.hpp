#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include "vdevice.hpp"


class VImgui : public VDevice
{
  public:
    VImgui();
    int minImageCount = 2;
    uint32_t queueFamily = (uint32_t)-1;
    VkDescriptorPool descriptorPool;
    bool swapChainRebuild = false;
    void initImgui();
    void cleanImgui();
    void frameRender(ImGui_ImplVulkanH_Window*, ImDrawData*);
    void framePresent(ImGui_ImplVulkanH_Window*);

  protected:
    VkResult createDescriptorPool();
    VkResult createSurfaceFormat(ImGui_ImplVulkanH_Window*, int, int);

};