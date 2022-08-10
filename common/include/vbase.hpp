#pragma once



#include "vdevice.hpp"


class VBase : public VDevice
{
  public:
    VBase();
    void run();
    void mainLoop();
    void initVulkan();    
    void cleanup();

  protected:
    
};

