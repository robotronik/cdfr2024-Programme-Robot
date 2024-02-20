#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <iostream>


#include "fonction.h"
#include "lidarPrint.h"
#include "lidar.h"






bool ctrl_c_pressed;
void ctrlc(int)
{
    ctrl_c_pressed = true;
}

int main() {

    lidarSetup("/dev/ttyUSB0",256000);

    signal(SIGINT, ctrlc);
    signal(SIGTERM, ctrlc);

    // fetech result and print it out...
    while (1) {
        
        lidarPrint();

        if (ctrl_c_pressed){ 
            break;
        }
    }

    lidarStop();

    return 0;
}