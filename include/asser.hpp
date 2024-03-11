#pragma once

extern "C" {
    #include <linux/i2c-dev.h>
    #include <i2c/smbus.h>
}

#include <cstdint>

#define I2C_ADDR 42

int asserTurnOnLed(int ledN);
int asserTurnOffLed(int ledN);

int assergetCoords(int &x,int &y,int &z);
int assersetCoords(int x,int y, int z);
int asserStop(void);
int asserLinearSetpoint(int x, int y);
int assertAngularSetpoint(int angle, int rotation);