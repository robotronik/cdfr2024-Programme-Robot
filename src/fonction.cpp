#include "fonction.h"


// Fonction pour mettre à jour l'état de la FSM en fonction de l'entrée
int initPositon(Asser* robot){
    static unsigned long startTime = millis()+3000;
    static int step = 0;

    robot->linearSetpoint(-100,0);
    if(step == 0 && startTime < millis()){
        robot->setCoords(-1500,0,0);
        step++;
        startTime = millis()+3000;
    }
    else if(step == 1 && startTime < millis()){
        robot->linearSetpoint(-1400,0);
        step++;
        startTime = millis()+3000;
    }
    else if(step == 2 && startTime < millis()){
        robot->angularSetpoint(60,0);
        step++;
        startTime = millis()+3000;
    }

}
