#pragma once



#include "vswapchain.hpp"


class VBase : public VSwapChain
{
  public:
    VBase();
    void run();
    void mainLoop();
    void initVulkan();    
    void cleanup();

  protected:
    
};

