#pragma once

extern "C" {
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
}

#include <iostream>

class I2CDevice {
   protected:
    int i2cFile;
    void generateBytes(int *values, size_t length, uint8_t *result);

   protected:
    I2CDevice(int slave_address);
};