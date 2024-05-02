#include "match.hpp"

int mainMatch(robotCDFR mainRobot, Asser* robot,Arduino* arduino){
    LOG_SCOPE("mainMatch");
    int ireturn = 0;
    static bool initStat = true;
    static fsmMatch_t currentState = MATCH_INIT;
    fsmMatch_t nextState = currentState;
    int deplacementreturn;


    switch (currentState)
    {
    case MATCH_INIT :
        if(initStat) LOG_STATE("MATCH_INIT");
        break;

    case MATCH_PLANT :
        if(initStat) LOG_STATE("MATCH_PLANT");
        break;


    case MATCH_SOLARPANEL :
        if(initStat) LOG_STATE("MATCH_SOLARPANEL");
        break;

    case MATCH_GOHOME1 :
        if(initStat) LOG_STATE("MATCH_GOHOME1");
        if(mainRobot.robotStatus.colorTeam == YELLOW){
            deplacementreturn = deplacementgoToPoint(mainRobot, robot,800,1250,-90,MOVE_FORWARD);
        }
        else{
            deplacementreturn = deplacementgoToPoint(mainRobot, robot,800,-1250,-90,MOVE_BACKWARD);
        }
        if(deplacementreturn<0){
            nextState = MATCH_GOHOME2;
        }
        else{
            nextState = MATCH_END;
        }
        break;

    case MATCH_GOHOME2 :
        if(initStat) LOG_STATE("MATCH_GOHOME2");
        if(mainRobot.robotStatus.colorTeam == YELLOW){
            //deplacementreturn = deplacementgoToPoint(mainRobot, robot,800,1250,-90,MOVE_FORWARD);
        }
        else{
            //deplacementreturn = deplacementgoToPoint(mainRobot, robot,800,-1250,-90,MOVE_BACKWARD);
        }
        if(deplacementreturn != 0){
            nextState = MATCH_END;
        }
        break;

    case MATCH_END :
        if(initStat) LOG_STATE("MATCH_END");
        break;
    
    
    default:
        if(initStat) LOG_ERROR("default");
        nextState = MATCH_INIT;
        break;
    }

    initStat = false;
    if(nextState != currentState){
        initStat = true;
    }
    currentState = nextState;
    return ireturn;

}