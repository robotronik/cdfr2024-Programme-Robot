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
        printf("catch plant\n");
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
        printf("release plant\n");
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