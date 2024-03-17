#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <iostream>


#include "fonction.h"
#include "lidarAnalize.h"
#include "lidar.h"
#include "conversionArray.h"

#define SIZEDATALIDAR 10000

bool ctrl_c_pressed;
void ctrlc(int)
{
    ctrl_c_pressed = true;
}

int main() {
    char sendData[50];

     for(int i =0; i<50; i++){
        sendData[i] = 0;
    }
    

    signal(SIGINT, ctrlc);
    signal(SIGTERM, ctrlc);

    // lidarAnalize_t lidarData[SIZEDATALIDAR];
    // position_t position = {0,0,0,0};
    // bool b_collidefordward;
    // bool b_collideBackward;
    
    // while (1) {
        
    //     int count = SIZEDATALIDAR;
    //     if(getlidarData(lidarData,count)){
    //         convertAngularToAxial(lidarData,count,position);
    //         printf("Colide backward : %d \t colide forward %d\n", b_collideBackward, b_collidefordward);
    //         //pixelArtPrint(lidarData, count,60,60,100,position);
    //         b_collidefordward = collideFordward(lidarData,count);
    //         b_collideBackward = collideBackward(lidarData,count);
    //     }


    //     if (ctrl_c_pressed){ 
    //         break;
    //     }
    // }



    //envoyer des valeurs
    //--------------------------------------------
    parameterToArray(sendData,50,"2%d 2%d 8%lld 4%lf",25,-25,9223372036854775800,0.2);//9223372036854775800
    //--------------------------------------------


    //SEND

    //print array
    for(int i =0; i<50; i++){
        printf("%d\t",sendData[i]);
        if(i%8 == 7)printf("\n");
    }
     printf("\n");


    //reception des valeurs
    int SmallPos;
    int SmallNeg;
    long long int llint;
    double valfloat;
    //--------------------------------------------
    arrayToParameter(sendData,50,"2%d 2%d 8%lld 4%lf",&SmallPos,&SmallNeg,&llint,&valfloat);
    //--------------------------------------------
    
    printf("%d %d %lld %lf\n",SmallPos,SmallNeg,llint,valfloat);

    return 0;
}