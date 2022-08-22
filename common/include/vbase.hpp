#pragma once

#include "vimgui.hpp"
#include "vframe.hpp"

class VBase : public VImgui
{
  public:
    VBase();
    void show(std::vector<u_char>*,  int* width, int* height);
    VFrame vFrame;
    std::vector<u_char>* frame;
     int* width;
     int* height;

  protected:
    
    void run();
    void mainLoop();
    void initVulkan();    
    void cleanup();

};

