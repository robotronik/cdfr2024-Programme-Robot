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

typedef enum {
    MOVING_FINISHED = 0,
    RUNNING_FINISHED = 1,
    TURNING_FINISHED = 2
} status_finished;

typedef enum{
    ANGULAR_ERROR = 0,
    LINEAR_ERROR = 1 
} asser_error_type;

// typedef enum{
//     TURN_CLOCKWISE = 
// }

class Asser {
   private:
    int i2cFile;

   public:
    Asser(int slave_address);
    int turnOnLed(int ledN);
    int turnOffLed(int ledN);

    int getCoords(int &x, int &y, int &theta);
    int setCoords(int x, int y, int z);
    int stop();
    int linearSetpoint(int x, int y);
    int angularSetpoint(int angle, int rotation);
    int setLookForward(int x, int y, int rotation);
    int setLookBackward(int x, int y, int rotation);
    // int getPostion(int &x, int &y, int &theta);
    int servo1Position(int position);
    bool getRobotFinished(status_finished status);
    int getError(asser_error_type error_type, int &error);
    int getBrakingDistance(int &distance);
};