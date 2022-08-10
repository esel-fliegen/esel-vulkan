#pragma once

#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "vkwindow.hpp"

class VkBase : public VkWindow
{
  public:
    VkBase();
    void run();
    void mainLoop();
};