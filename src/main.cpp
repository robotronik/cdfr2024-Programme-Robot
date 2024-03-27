#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <iostream>

#include "fonction.h"
#include "lidarAnalize.h"
#include "lidar.h"
#include "asser.hpp"
#include "arduino.hpp"
#include "utils.h"

#define SIZEDATALIDAR 10000



bool ctrl_c_pressed;
void ctrlc(int)
{
    ctrl_c_pressed = true;
}

int main() {

    if(!lidarSetup("/dev/ttyAMA0",256000)){
        return -1;
    }

    signal(SIGINT, ctrlc);
    signal(SIGTERM, ctrlc);

    lidarAnalize_t lidarData[SIZEDATALIDAR];
    position_t position = {0,0,0,0};
    bool b_collidefordward;
    bool b_collideBackward;
    Asser *robot = new Asser(I2C_ASSER_ADDR);
    Arduino *arduino = new Arduino(100);

    while (1) {
        
        int count = SIZEDATALIDAR;
        if(getlidarData(lidarData,count)){
            convertAngularToAxial(lidarData,count,position);
            b_collidefordward = collideFordward(lidarData,count);
            b_collideBackward = collideBackward(lidarData,count);
        }

        turnSolarPannel(robot);

        if (ctrl_c_pressed){ 
            break;
        }
    }

    robot->stop();
    lidarStop();

    return 0;
}