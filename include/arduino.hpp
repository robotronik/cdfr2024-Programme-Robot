#pragma once

#include <cstdint>
#include <iostream>
#include "I2CDevice.hpp"

class Arduino : public I2CDevice {
    using I2CDevice::I2CDevice;
   
   public:
    Arduino(int slave_address);
    int servo1Position(int position);
<<<<<<< Updated upstream
    int readCapteur(int capteurNumber, int &state);
=======
    bool getStartSignal();
    bool getTeamColor();
>>>>>>> Stashed changes
};