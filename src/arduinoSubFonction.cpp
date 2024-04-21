#include "arduinoSubFonction.h"

int pullpush(Arduino* arduino){
    static unsigned long startTime;
    static int step = 0;
    bool bret = false;

    if(step == 0 ){
        arduino->servoPosition(1,0);
        step++;
        startTime = millis()+350;
    }
    else if(step == 1 && startTime < millis()){
        arduino->servoPosition(1,180);
        step++;
        startTime = millis()+350;
    }
    else if(step == 2 && startTime < millis()){
        step = 0;
        bret = true;
    }
    return bret;    
}

int catchPlant(Arduino* arduino){
    static unsigned long startTime;
    static int step = 0;
    bool bret = false;

    if(step == 0 ){
        arduino->servoPosition(2,0);
        step++;
        startTime = millis()+350;
    }
    else if(step == 1 && startTime < millis()){
        step = 0;
        bret = true;
    }
    return bret;
}
int releasePlant(Arduino* arduino){
    static unsigned long startTime;
    static int step = 0;
    bool bret = false;

    if(step == 0 ){
        arduino->servoPosition(2,180);
        step++;
        startTime = millis()+350;
    }
    else if(step == 1 && startTime < millis()){
        step = 0;
        bret = true;
    }
    return bret;
}