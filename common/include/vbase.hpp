#pragma once

#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "vdevice.hpp"

class VBase : public VDevice
{
  public:
    VBase();
    void run();
    void mainLoop();
    void cleanup();
};