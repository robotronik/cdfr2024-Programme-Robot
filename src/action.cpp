#include "action.hpp"


action::action(robotCDFR* imainRobot, Asser* irobot, Arduino* iarduino, tableState* itable){
    mainRobot = imainRobot;
    robot = irobot;
    arduino = iarduino;
    table = itable;

    noEndPoint = true;
    currentState = FSMACTION_INIT;
    initStat = true;
}

action::~action(){
   
}

int action::runAction(void){
    LOG_SCOPE("Action");
    int ireturn = 0;
    fsmAction_t nextState = currentState;
    int deplacementreturn;

    switch (currentState)
    {
    case FSMACTION_INIT :
        if(initStat) LOG_STATE("FSMACTION_INIT");
        nextState = FSMACTION_MOVESTART;
        break;

    case FSMACTION_MOVESTART :
        if(initStat) LOG_STATE("FSMACTION_MOVESTART");
        deplacementreturn = goToStart();
        if(deplacementreturn>0){
            nextState = FSMACTION_ACTION;
        }
        else if(deplacementreturn<0){
            nextState = FSMACTION_INIT;
            return -1;
        }
        break;


    case FSMACTION_ACTION :
        if(initStat) LOG_STATE("FSMACTION_ACTION");
        deplacementreturn = runActionPtr(this,mainRobot,robot,arduino,table);
        if(deplacementreturn>0){
            if(noEndPoint){
                nextState = FSMACTION_INIT;
                return 1;
            }
            else{
                nextState = FSMACTION_MOVEEND;
            }
        }
        else if(deplacementreturn<0){
            nextState = FSMACTION_INIT;
            return -1;
        }
        break;


    case FSMACTION_MOVEEND :
        if(initStat) LOG_STATE("FSMACTION_MOVEEND");
        deplacementreturn = goToEnd();
        if(deplacementreturn>0){
            nextState = FSMACTION_INIT;
            return 1;
        }
        else if(deplacementreturn<0){
            nextState = FSMACTION_INIT;
            return -1;
        }
        break;
        
    
    default:
        if(initStat) LOG_STATE("default");
        nextState = FSMACTION_INIT;
        break;
    }

    initStat = false;
    if(nextState != currentState){
        initStat = true;
    }
    currentState = nextState;
    return ireturn;
}

int action::validAction(void){
    return validActionPtr(table);
}

void action::setvalidAction(std::function<int(tableState*)> ptr){
    validActionPtr = ptr;
}

void action::setRunAction(std::function<int(action*, robotCDFR*, Asser*, Arduino*, tableState*)> ptr){
    runActionPtr = ptr;
}

int action::goToStart(void){
    return deplacementgoToPoint(*mainRobot, robot, startPostion.x,startPostion.y, startPostion.teta, startDirection);
}


int action::goToEnd(void){
    return deplacementgoToPoint(*mainRobot, robot, endPostion.x, endPostion.y, endPostion.teta, endDirection);
}

void action::setStartPoint(int x, int y, int teta, asser_direction_side Direction, asser_rotation_side rotation){
    startPostion.x = x;
    startPostion.y = y;
    startPostion.teta = teta;
    startDirection = Direction;
    startRotation = rotation;
}
void action::setEndPoint(int x, int y, int teta, asser_direction_side Direction, asser_rotation_side rotation){
    endPostion.x = x;
    endPostion.y = y;
    endPostion.teta = teta;
    endDirection = Direction;
    endRotation = rotation;
    noEndPoint = false;
}