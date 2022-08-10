#pragma once

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <string>

#include "vwindow.hpp"


class VDevice : public VWindow 
{
  public: 
    VDevice();
    
    VkInstance instance;

  protected:
    void createInstance();
    void vkResult(VkResult, const char*);

};