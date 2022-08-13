#pragma once

#include "vimgui.hpp"

class VBase : public VImgui
{
  public:
    VBase();
    void run();
    void mainLoop();
    void initVulkan();    
    void cleanup();

  protected:
    
};

