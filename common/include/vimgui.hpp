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
    ImGui_ImplVulkanH_Window mainWindowData;
    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
    VkPipelineCache pipelineCache = VK_NULL_HANDLE;
    bool swapChainRebuild = false;

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    void initImgui();
    void cleanImgui();
    void frameRender(ImGui_ImplVulkanH_Window*, ImDrawData*);
    void framePresent(ImGui_ImplVulkanH_Window*);
    void renderLoopBegin();
    void renderLoopEnd(ImGui_ImplVulkanH_Window*);

  protected:
    VkResult createDescriptorPool();
    VkResult createSurfaceFormat(ImGui_ImplVulkanH_Window*, int, int);
    void imguiDemo();

};

static void check_vk_result(VkResult err)
{
    if (err == 0)
        return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0)
        abort();
}