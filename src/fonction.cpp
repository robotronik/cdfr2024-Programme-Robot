#include "fonction.h"


// Fonction pour mettre à jour l'état de la FSM en fonction de l'entrée
int initPositon(Asser* robot){
    static unsigned long startTime = millis()+2000;
    static int step = 0;

    if(step == 0 && startTime < millis()){
        robot->linearSetpoint(-300,0);
        printf("1\n");
        step++;
        startTime = millis()+2000;
    }
    else if(step == 1 && startTime < millis()){
        robot->setCoords(-1500,0,0);
        printf("2\n");
        step++;
        startTime = millis()+2000;
    }
    else if(step == 2 && startTime < millis()){
        robot->linearSetpoint(-1400,0);
        robot->stop();
        printf("3\n");
        step++;
        startTime = millis()+2000;
    }

}

int turnSolarPannel(Asser* robot){
    static unsigned long startTime = millis()+2000;
    static int step = 0;

    //printf(" %d\n",robot->getError(LINEAR_ERROR));

    if(step == 0 && startTime < millis()){
        robot->linearSetpoint(100,0);
        printf("1\n");
        step++;
        startTime = millis()+2000;
    }
    else if(step == 1 && !robot->getError(LINEAR_ERROR)){
        robot->linearSetpoint(200,0);
        printf("2\n");
        step++;
        startTime = millis()+2000;
    }
    else if(step == 2 && !robot->getError(LINEAR_ERROR)){
        robot->linearSetpoint(300,0);
        printf("3\n");
        step++;
        startTime = millis()+2000;
    }

}
