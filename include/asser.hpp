#pragma once

extern "C" {
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
}

#include <cstdint>
#include <iostream>

#define I2C_ASSER_ADDR 42

class Asser {
   private:
    int i2cFile;

   public:
    Asser(int slave_address);
    int turnOnLed(int ledN);
    int turnOffLed(int ledN);

    int getCoords(int &x, int &y, int &z);
    int setCoords(int x, int y, int z);
    int stop();
    int linearSetpoint(int x, int y);
    int angularSetpoint(int angle, int rotation);
    int setLookForward(int x, int y, int rotation);
    int setLookBackward(int x, int y, int rotation);
};