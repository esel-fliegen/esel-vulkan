#include "vimgui.hpp"

VImgui::VImgui()
{
  
}

void VImgui::framePresent(ImGui_ImplVulkanH_Window* wd)
{
  if (swapChainRebuild)
        return;
  VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
  VkPresentInfoKHR info = {};
  info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  info.waitSemaphoreCount = 1;
  info.pWaitSemaphores = &render_complete_semaphore;
  info.swapchainCount = 1;
  info.pSwapchains = &wd->Swapchain;
  info.pImageIndices = &wd->FrameIndex;
  VkResult result = vkResult(vkQueuePresentKHR(graphicsQueue, &info),
    "queue present");
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
  {
    swapChainRebuild = true;
    return;
  }
  wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->ImageCount;
}

void VImgui::frameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data)
{
  VkSemaphore image_acquired_semaphore  = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
  VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
  VkResult result = vkResult(vkAcquireNextImageKHR(logicalDevice, wd->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &wd->FrameIndex), 
    "acquire next image");
  
  if ( result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
  {
    swapChainRebuild = true;
    return;
  }

  ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
  {
    result = vkResult(vkWaitForFences(logicalDevice, 1, &fd->Fence, VK_TRUE, UINT64_MAX),
      "wait for fences");
    result = vkResult(vkResetFences(logicalDevice, 1, &fd->Fence), 
      "reset fences");
  }

  {
    result = vkResult(vkResetCommandPool(logicalDevice, fd->CommandPool, 0),
      "reset command pool");
    VkCommandBufferBeginInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    result = vkResult(vkBeginCommandBuffer(fd->CommandBuffer, &info),
      "begin command buffer");
  }

  {
    VkRenderPassBeginInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    info.renderPass = wd->RenderPass;
    info.framebuffer = fd->Framebuffer;
    info.renderArea.extent.width = wd->Width;
    info.renderArea.extent.height = wd->Height;
    info.clearValueCount = 1;
    info.pClearValues = &wd->ClearValue;
    vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
  }

  ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

  vkCmdEndRenderPass(fd->CommandBuffer);
  {
    VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = &image_acquired_semaphore;
    info.pWaitDstStageMask = &wait_stage;
    info.commandBufferCount = 1;
    info.pCommandBuffers = &fd->CommandBuffer;
    info.signalSemaphoreCount = 1;
    info.pSignalSemaphores = &render_complete_semaphore;

    result = vkResult(vkEndCommandBuffer(fd->CommandBuffer),
      "end command buffer");
    result = vkResult(vkQueueSubmit(graphicsQueue, 1, &info, fd->Fence),
      "queue submit");
  }
}

VkResult VImgui::createSurfaceFormat(ImGui_ImplVulkanH_Window* wd, int width, int height)
{
  wd->Surface = surface;
  VkBool32 res;
  VkResult result;

  const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
  const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
  wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(physicalDevice, wd->Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);

  #ifdef IMGUI_UNLIMITED_FRAME_RATE
    VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
#else
    VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
#endif
    wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(physicalDevice, wd->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));

    IM_ASSERT(minImageCount >= 2);
    ImGui_ImplVulkanH_CreateOrResizeWindow(instance, physicalDevice, logicalDevice, wd, queueFamilyIndices.graphicsFamily.value(),
      VK_NULL_HANDLE, width, height, minImageCount);

  return result;
}

VkResult VImgui::createDescriptorPool()
{
  VkDescriptorPoolSize pool_sizes[] =
  {
    { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
    { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
    { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
    { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
    { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
    { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
    { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
    { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
    { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
    { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
    { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
  };

  VkDescriptorPoolCreateInfo pool_info = {};
  pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
  pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
  pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
  pool_info.pPoolSizes = pool_sizes;
  VkResult result = vkResult(vkCreateDescriptorPool(logicalDevice, &pool_info, VK_NULL_HANDLE, &descriptorPool),
    "create descriptor pool");
  
  return result;
}

void VImgui::renderLoopBegin()
{
  if (swapChainRebuild)
  {
    int width, height;
    glfwGetFramebufferSize(glfwWindow, &width, &height);
    if (width > 0 && height > 0)
    {
      ImGui_ImplVulkan_SetMinImageCount(minImageCount);
      ImGui_ImplVulkanH_CreateOrResizeWindow(instance, physicalDevice, logicalDevice, &mainWindowData, queueFamilyIndices.graphicsFamily.value(),
        VK_NULL_HANDLE, width, height, minImageCount);
      mainWindowData.FrameIndex = 0;
      swapChainRebuild = false;
    }
  }

  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void VImgui::renderLoopEnd(ImGui_ImplVulkanH_Window* wd)
{
  ImGui::Render();
  ImDrawData* draw_data = ImGui::GetDrawData();
  const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
  if (!is_minimized)
  {
    wd->ClearValue.color.float32[0] = clear_color.x * clear_color.w;
    wd->ClearValue.color.float32[1] = clear_color.y * clear_color.w;
    wd->ClearValue.color.float32[2] = clear_color.z * clear_color.w;
    wd->ClearValue.color.float32[3] = clear_color.w;
    frameRender(wd, draw_data);
    framePresent(wd);
  }
}



void VImgui::initImgui()
{
  int w, h;
  glfwGetFramebufferSize(glfwWindow, &w, &h);
  ImGui_ImplVulkanH_Window* wd = &mainWindowData;
  createDescriptorPool();

  createSurfaceFormat(wd, w, h);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForVulkan(glfwWindow, true);
  ImGui_ImplVulkan_InitInfo init_info = {};
  init_info.Instance = instance;
  init_info.PhysicalDevice = physicalDevice;
  init_info.Device = logicalDevice;
  init_info.QueueFamily = queueFamilyIndices.graphicsFamily.value();
  init_info.Queue = graphicsQueue;
  init_info.PipelineCache = pipelineCache;
  init_info.DescriptorPool = descriptorPool;
  init_info.Subpass = 0;
  init_info.MinImageCount = minImageCount;
  init_info.ImageCount = wd->ImageCount;
  init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
  init_info.Allocator = VK_NULL_HANDLE;
  init_info.CheckVkResultFn = check_vk_result;
  ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);

  {
    VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;
    VkCommandBuffer command_buffer = wd->Frames[wd->FrameIndex].CommandBuffer;

    VkResult result = vkResult(vkResetCommandPool(logicalDevice, command_pool, 0),
      "reset command pool");
    
    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    result = vkResult(vkBeginCommandBuffer(command_buffer, &begin_info),
      "begin command buffer");

    ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

    VkSubmitInfo end_info = {};
    end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    end_info.commandBufferCount = 1;
    end_info.pCommandBuffers = &command_buffer;
    result = vkResult(vkEndCommandBuffer(command_buffer), 
      "end command buffer");
    result = vkResult(vkQueueSubmit(graphicsQueue, 1, &end_info, VK_NULL_HANDLE),
      "queue submit");

    result = vkResult(vkDeviceWaitIdle(logicalDevice),
      "device wait idle");

    ImGui_ImplVulkan_DestroyFontUploadObjects();
  }
}

void VImgui::cleanImgui()
{
  vkResult(vkDeviceWaitIdle(logicalDevice),
    "device wait idle");  
  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  ImGui_ImplVulkanH_DestroyWindow(instance, logicalDevice, &mainWindowData, VK_NULL_HANDLE);
  vkDestroyDescriptorPool(logicalDevice, descriptorPool, VK_NULL_HANDLE);
}

void VImgui::imguiDemo()
{
  if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
}