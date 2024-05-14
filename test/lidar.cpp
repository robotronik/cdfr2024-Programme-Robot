#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <iostream>


#include "lidarAnalize.h"
#include "lidar.h"

//#define DISABLE_LIDAR
#define SIZEDATALIDAR 10000

typedef enum {
    INIT,
    INITIALIZE,
    SETHOME,
    WAITSTART,
    START,
    RUN,
    RETURNHOME,
    FIN,
    STOP
} main_State_t;

bool ctrl_c_pressed;
void ctrlc(int)
{
    ctrl_c_pressed = true;
}
bool ctrl_z_pressed = false;
void ctrlz(int signal) {
    ctrl_z_pressed = true;
}

void executePythonScript(const std::string& command) {
    std::system(command.c_str());
}

int main(int argc, char *argv[]) {
    LOG_INIT();


    if(!lidarSetup("/dev/ttyAMA0",256000)){
        LOG_ERROR("cannot find the lidar");
        return -1;
    }



    signal(SIGINT, ctrlc);
    signal(SIGTERM, ctrlc);
    //signal(SIGTSTP, ctrlz);
    
    lidarAnalize_t lidarData[SIZEDATALIDAR];    



    while (1) {

        sleep(0.01);
        
       
        int count = SIZEDATALIDAR;
        if(getlidarData(lidarData,count)){
            int distance = 0;
            position_t position = {0,0,0,0};
            convertAngularToAxial(lidarData,count,position);
            if(ctrl_z_pressed){
                ctrl_z_pressed = false;
                pixelArtPrint(lidarData,count,50,50,100,position);
            }
            collide(lidarData,count,distance);
        }
       

        if (ctrl_c_pressed){ 
            break;
        }
    }
    
    lidarStop();

    return 0;
}
