#include "arduinoSubFonction.h"
#include "logger.hpp"

int pullpush(Arduino* arduino){
    static unsigned long startTime;
    static int step = 0;
    bool bret = false;

    if(step == 0 ){
        LOG_INFO("push pull");
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
        LOG_INFO("catch plant");
        arduino->servoPosition(2,CLAMPOPEN);
        step++;
        startTime = millis()+350;
    }
    else if(step == 1 && startTime < millis()){
        arduino->moveStepper(ELEVATORPLANT,1);
        step++;
        startTime = millis()+DELAYUPDOWN;
    }
    else if(step == 2 && startTime < millis()){
        arduino->servoPosition(2,CLAMPCLOSE);
        step++;
        startTime = millis()+350;
    }
    else if(step == 3 && startTime < millis()){
        arduino->moveStepper(ELEVATORUP,1);
        step++;
        startTime = millis()+DELAYUPDOWN;
    }
    else if(step == 4 && startTime < millis()){
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
        LOG_INFO("release plant");
        arduino->moveStepper(ELEVATORPLANT,1);
        step++;
        startTime = millis()+DELAYUPDOWN;
    }
    else if(step == 1 && startTime < millis()){
        arduino->servoPosition(2,CLAMPOPEN);
        step++;
        startTime = millis()+350;
    }
    else if(step == 2 && startTime < millis()){
        arduino->moveStepper(ELEVATORUP,1);
        step++;
        startTime = millis()+DELAYUPDOWN;
    }
    else if(step == 3 && startTime < millis()){
        arduino->servoPosition(2,CLAMPSLEEP);
        step++;
        startTime = millis()+350;
    }
    else if(step == 4 && startTime < millis()){
        step = 0;
        bret = true;
    }
    return bret;
}

void blinkLed(Arduino* arduino,int LedNb,int periode){
    static unsigned long startTime = millis();
    static int step = 0;

    if(step == 0 && startTime < millis()){
        arduino->ledOn(LedNb);
        step++;
        startTime = millis()+periode/2;
    }
    else if(step == 1 && startTime < millis()){
        arduino->ledOff(LedNb);
        step = 0;
        startTime = millis()+periode/2;
    }
    return;
}