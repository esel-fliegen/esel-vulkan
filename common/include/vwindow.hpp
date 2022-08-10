#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;

class VWindow{
  public:
    GLFWwindow* glfwWindow;

    VWindow();
    void initGLFWwindow();
    void processInput(GLFWwindow*);
    void cleanupGLFWwindow();

};