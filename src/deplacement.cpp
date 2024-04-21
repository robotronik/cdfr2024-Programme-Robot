#include "deplacement.h"



int deplacementLinearPoint(Asser* robot, int x, int y){
    static unsigned long startTime;
    static int memx;
    static int memy;
    static deplcement_State_t step = DEPLACEMENT_INIT;
    int iret = 0;
    int distance;

    switch (step)
    {
    case DEPLACEMENT_INIT:
        printf("=>DeplacementState : INIT\n");
        memx = y;
        memy = x;
        if(robot->collide < DISTANCESTOP){
            step = DEPLACEMENT_WAIT;
            startTime = millis() + 5000; //TIME waiting
        }
        else{
            step = DEPLACEMENT_MOVE;
            robot->linearSetpoint(memx,memy);
        }
        break;
    
    case DEPLACEMENT_MOVE:
        printf("=>DeplacementState : MOVE\n");
        if(!robot->getError(ANGULAR_ERROR)){
            step = DEPLACEMENT_INIT;
            iret = 1; //GOOD END
        }
        if(robot->collide < DISTANCESTOP){
            step = DEPLACEMENT_STOP;
            robot->stop();
        }
        break;

    case DEPLACEMENT_STOP:
        printf("=>DeplacementState : STOP\n");
        robot->getBrakingDistance(distance);
        if(distance==0){
            startTime = millis() + 5000;
            step = DEPLACEMENT_WAIT;
        }
        break;

    case DEPLACEMENT_WAIT:
        printf("=>DeplacementState : WAIT\n");
        if(startTime < millis()){
            step = DEPLACEMENT_INIT;
            iret = -1; //BAD END
        }
        if(robot->collide > DISTANCERESTART){
            step = DEPLACEMENT_MOVE;
            robot->linearSetpoint(memx,memy);
        }
        break;
    
    default:
        step = DEPLACEMENT_INIT;
        break;
    }

    return iret;
}