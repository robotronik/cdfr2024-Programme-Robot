#include "deplacement.h"



int deplacementLinearPoint(Asser* robot, int x, int y){
    static unsigned long startTime;
    static int memx;
    static int memy;
    static bool initStat = true; 
    static deplcement_State_t step = DEPLACEMENT_INIT;
    deplcement_State_t nextstep = step;
    int iret = 0;
    int distance;


    switch (step)
    {
    case DEPLACEMENT_INIT:
        if(initStat) printf("=> DeplacementState : INIT\n");
        memx = x;
        memy = y;
        if(robot->collide < DISTANCESTOP){
            printf("distance colide : %d\n",robot->collide);
            nextstep = DEPLACEMENT_WAIT;
            startTime = millis() + 5000; //TIME waiting
        }
        else{
            nextstep = DEPLACEMENT_MOVE;
            robot->linearSetpoint(memx,memy);
        }
        break;
    
    case DEPLACEMENT_MOVE:
        if(initStat) printf("=> DeplacementState : MOVE\n");
        if(!robot->getError(ANGULAR_ERROR)){
            nextstep = DEPLACEMENT_INIT;
            iret = 1; //GOOD END
        }
        if(robot->collide < DISTANCESTOP){
            printf("distance colide : %d\n",robot->collide);
            nextstep = DEPLACEMENT_STOP;
            robot->stop();
        }
        break;

    case DEPLACEMENT_STOP:
        if(initStat) printf("=> DeplacementState : STOP\n");
        robot->getBrakingDistance(distance);
        if(distance==0){
            startTime = millis() + 5000;
            nextstep = DEPLACEMENT_WAIT;
        }
        break;

    case DEPLACEMENT_WAIT:
        if(initStat) printf("=> DeplacementState : WAIT\n");
        if(startTime < millis()){
            nextstep = DEPLACEMENT_INIT;
            iret = -1; //BAD END
        }
        if(robot->collide > DISTANCERESTART){
            printf("distance colide : %d\n",robot->collide);
            nextstep = DEPLACEMENT_MOVE;
            robot->linearSetpoint(memx,memy);
        }
        break;
    
    default:
        printf("=> DeplacementState : default\n");
        nextstep = DEPLACEMENT_INIT;
        break;
    }

    initStat = false;
    if(nextstep != step){
        initStat = true;
    }
    step = nextstep;
    return iret;
}