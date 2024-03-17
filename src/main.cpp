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
    position_t position = {0,0,0,0};
    bool b_collidefordward;
    bool b_collideBackward;
    
    while (1) {
        
        int count = SIZEDATALIDAR;
        if(getlidarData(lidarData,count)){
            convertAngularToAxial(lidarData,count,position);
            printf("Colide backward : %d \t colide forward %d\n", b_collideBackward, b_collidefordward);
            //pixelArtPrint(lidarData, count,60,60,100,position);
            b_collidefordward = collideFordward(lidarData,count);
            b_collideBackward = collideBackward(lidarData,count);
        }


        if (ctrl_c_pressed){ 
            break;
        }
    }

    lidarStop();

    return 0;
}