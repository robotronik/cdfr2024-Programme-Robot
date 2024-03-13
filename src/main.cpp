#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <iostream>


#include "fonction.h"
#include "lidarAnalize.h"
#include "lidar.h"

#define SIZEDATALIDAR 10000

bool ctrl_c_pressed;
void ctrlc(int)
{
    ctrl_c_pressed = true;
}

int main() {

    if(!lidarSetup("/dev/ttyUSB0",256000)){
        return -1;
    }

    signal(SIGINT, ctrlc);
    signal(SIGTERM, ctrlc);

    lidarAnalize_t lidarData[SIZEDATALIDAR];
    position_t position = {1000,0,15,0};
    // fetech result and print it out...
    while (1) {
        
        int count = SIZEDATALIDAR;
        if(getlidarData(lidarData,count)){
            //printAngular(lidarData,count);
            convertAngularToAxial(lidarData,count,position);
            pixelArtPrint(lidarData, count,60,60,100,position);
            //ctrl_c_pressed = true;
        }


        if (ctrl_c_pressed){ 
            break;
        }
    }

    lidarStop();

    return 0;
}