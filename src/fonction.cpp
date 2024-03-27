#include "fonction.h"


// Fonction pour mettre à jour l'état de la FSM en fonction de l'entrée
int initPositon(Asser* robot){
    static unsigned long startTime;
    static int step = -1;
    //printf(" %d\n",robot->getError(LINEAR_ERROR));

    if(step == -1){
        robot->setCoords(1000-ROBOT_X_OFFSET-150,-1500+ROBOT_Y_OFFSET+150,270);
        printf("-1\n");
        step++;
        startTime = millis()+6000;
    }
    else if(step == 0 && startTime < millis()){
        robot->linearSetpoint(1000-ROBOT_X_OFFSET-150,-1500+ROBOT_Y_OFFSET);
        printf("0\n");
        step++;
        startTime = millis()+000;
    }
    else if(step == 1 && startTime < millis()){
        robot->setCoords(1000-ROBOT_X_OFFSET-100,-1500+ROBOT_Y_OFFSET,-90);
        robot->linearSetpoint(1000-ROBOT_X_OFFSET-150, -1500+ROBOT_Y_OFFSET+150);
        printf("1\n");
        step++;
    }
    else if(step == 2 && !robot->getError(LINEAR_ERROR)){
        robot->angularSetpoint(180,0);
        printf("2\n");
        step++;
    }
    else if(step == 3 && !robot->getError(ANGULAR_ERROR)){
        robot->linearSetpoint(1000-ROBOT_X_OFFSET,-1500+ROBOT_Y_OFFSET+150);
        printf("3\n");
        startTime = millis()+2000;
        step++;
    }
    else if(step == 4 && startTime < millis()){
        robot->setCoords(1000-ROBOT_X_OFFSET,-1500+ROBOT_Y_OFFSET+150,180);
        robot->linearSetpoint(1000-ROBOT_X_OFFSET-100,-1500+ROBOT_Y_OFFSET+150);
        printf("4\n");
        step++;
    }
    else if(step == 5 && !robot->getError(LINEAR_ERROR)){
        robot->angularSetpoint(-90,0);
        printf("5\n");
        step++;
    }
    else if(step == 6 && !robot->getError(ANGULAR_ERROR)){
        printf("6\n");
        step++;
    }
}

int pullpush(Arduino* arduino){
    static unsigned long startTime;
    static int step = 0;
    bool bret = false;

    if(step == 0 ){
        arduino->servo1Position(0);
        printf("0\n");
        step++;
        startTime = millis()+350;
    }
    else if(step == 1 && startTime < millis()){
        arduino->servo1Position(180);
        printf("1\n");
        step++;
        startTime = millis()+350;
    }
    else if(step == 2 && startTime < millis()){
        step = 0;
        bret = true;
    }
    return bret;    
}

int turnSolarPannel(Asser* robot,Arduino* arduino){
    static unsigned long startTime;
    static int step = 0;
    static int loop = 0;
    const int table[6] = {-50,-275,-400,-900,-1125,-1350};

    if(step == 0){
        robot->linearSetpoint(table[loop],0);
        step++;   
    }
    else if(step == 1 && !robot->getError(LINEAR_ERROR)){
        step++;
    }
    else if(step == 2){
        if(pullpush(arduino)){
            robot->linearSetpoint(table[loop]-10,0);
            step++;
        }

    }
    else if(step == 3 && !robot->getError(LINEAR_ERROR)){
        step++;
    }
     else if(step == 4){
        if(pullpush(arduino)){
            loop++;
            step=0;
        }
    }

    bool breturn = false;
    if(loop == 6){
        loop = 0;
    }
    return breturn;

}
