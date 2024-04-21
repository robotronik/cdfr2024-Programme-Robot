#include "deplacement.h"



int deplacementLinearPoint(Asser* robot, int x, int y){
    static unsigned long startTime;
    static int memx;
    static int memy;
    static deplcement_State_t step = DEPLACEMENT_INIT;
    deplcement_State_t nextstep = step;
    int iret = 0;
    int distance;


    switch (step)
    {
    case DEPLACEMENT_INIT:
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
        robot->getBrakingDistance(distance);
        if(distance==0){
            startTime = millis() + 5000;
            nextstep = DEPLACEMENT_WAIT;
        }
        break;

    case DEPLACEMENT_WAIT:
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
        nextstep = DEPLACEMENT_INIT;
        break;
    }

    if(nextstep != step){
        switch (nextstep)
        {
        case DEPLACEMENT_INIT:
            printf("=>DeplacementState : INIT\n");
            break;
        
        case DEPLACEMENT_MOVE:
            printf("=>DeplacementState : MOVE\n");
            break;

        case DEPLACEMENT_STOP:
            printf("=>DeplacementState : STOP\n");
            break;

        case DEPLACEMENT_WAIT:
            printf("=>DeplacementState : WAIT\n");
            break;
        
        default:
            printf("=>DeplacementState : default\n");
            break;
        }
    }

    step = nextstep;

    return iret;
}