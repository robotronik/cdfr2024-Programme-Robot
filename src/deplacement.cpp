#include "deplacement.h"



int deplacementLinearPoint(robot mainRobot, Asser* robot, int x, int y){
    LOG_SCOPE("MOVE");
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
        if(initStat) LOG_STATE("DEPLACEMENT_INIT");
        memx = x;
        memy = y;
        if(mainRobot.robotStatus.collide < DISTANCESTOP){
            printf("distance colide : %d\n",mainRobot.robotStatus.collide);
            nextstep = DEPLACEMENT_WAIT;
            startTime = millis() + 5000; //TIME waiting
        }
        else{
            nextstep = DEPLACEMENT_MOVE;
            robot->linearSetpoint(memx,memy);
        }
        break;
    
    case DEPLACEMENT_MOVE:
        if(initStat) LOG_STATE("DEPLACEMENT_MOVEt");
        if(!robot->getError(LINEAR_ERROR)){
            nextstep = DEPLACEMENT_INIT;
            iret = 1; //GOOD END
        }
        if(mainRobot.robotStatus.collide < DISTANCESTOP){
            printf("distance colide : %d\n",mainRobot.robotStatus.collide);
            nextstep = DEPLACEMENT_STOP;
            robot->stop();
        }
        break;

    case DEPLACEMENT_STOP:
        if(initStat) LOG_STATE("DEPLACEMENT_STOP");
        robot->getBrakingDistance(distance);
        if(distance==0){
            startTime = millis() + 5000;
            nextstep = DEPLACEMENT_WAIT;
        }
        break;

    case DEPLACEMENT_WAIT:
        if(initStat) LOG_STATE("DEPLACEMENT_WAIT");
        if(startTime < millis()){
            nextstep = DEPLACEMENT_INIT;
            iret = -1; //BAD END
        }
        if(mainRobot.robotStatus.collide > DISTANCERESTART){
            printf("distance colide : %d\n",mainRobot.robotStatus.collide);
            nextstep = DEPLACEMENT_MOVE;
            robot->linearSetpoint(memx,memy);
        }
        break;
    
    default:
        LOG_ERROR("DEFAULT");
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