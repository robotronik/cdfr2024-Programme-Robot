#include "fonction.h"

fsmMatch_t currentStateMatch = SOLARPANNEL;

// Fonction pour mettre à jour l'état de la FSM en fonction de l'entrée
int initPositon(Asser* robot,int x, int y,int teta){
    static unsigned long startTime;
    static int step = -1;
    //printf(" %d\n",robot->getError(LINEAR_ERROR));

    int TetaStart = 90;
    int TetaSecond = -180;
    int xSecond = 200;
    int xStart = 1000 - ROBOT_Y_OFFSET;
    int yStart = 1500 - ROBOT_Y_OFFSET;
    if(y<0){
        TetaStart = -90;
    }
    if(y<0){
        yStart = -yStart;
    }
    if(x<0){
        TetaSecond = 0;
        xSecond = -xSecond;
        xStart = -xStart;
    }

    if(step == -1){
        robot->setCoords(0,0,0);
        printf("-1\n");
        step++;
        startTime = millis()+1000;
    }
    else if(step == 0 && startTime < millis()){
        robot->linearSetpoint(-150,0);
        printf("0\n");
        step++;
        startTime = millis()+2000;
    }
    else if(step == 1 && startTime < millis()){
        robot->setCoords(0,yStart,TetaStart);
        robot->linearSetpoint(0,y);
        printf("1\n");
        step++;
    }
    else if(step == 2 && !robot->getError(LINEAR_ERROR)){
        robot->angularSetpoint(TetaSecond,0);
        printf("2\n");
        step++;
    }
    else if(step == 3 && !robot->getError(ANGULAR_ERROR)){
        robot->linearSetpoint(xSecond,y);
        printf("3\n");
        startTime = millis()+2000;
        step++;
    }
    else if(step == 4 && startTime < millis()){
        robot->setCoords(xStart, y,TetaSecond);
        robot->linearSetpoint(x , y);
        printf("4\n");
        step++;
    }
    else if(step == 5 && !robot->getError(LINEAR_ERROR)){
        robot->angularSetpoint(teta,0);
        printf("5\n");
        step++;
    }
    else if(step == 6 && !robot->getError(ANGULAR_ERROR)){
        printf("6\n");
        step++;
    }
    return step>6;
}



int turnSolarPannel(Asser* robot,Arduino* arduino){
    LOG_SCOPE("SolarPanel");
    static fsmSolarPanel_t currentState = SOLARPANEL_INIT;
    fsmSolarPanel_t nextState = currentState;
    static bool initStat = true;
    int ireturn = 0;


    static unsigned long startTime;
    static int step = 0;
    static int loop = 0;
    const int offsetRobotYellow1 = 5;
    const int offsetRobotYellow2 = 15;
    const int table[9] = {1225,1000,775,225,0,-225,-775,-1000,-1225};
    const int axeX = 800;
    int returnValDeplacement;
    //const int table[6] = {-50,-275,-400,-900,-1125,-1350};

    switch (currentState)
    {
    case SOLARPANEL_INIT :
        break;

    case SOLARPANEL_FORWARD :
        break;

    case SOLARPANEL_PUSHFOR :
        break;

    case SOLARPANEL_BACKWARD :
        break;

    case SOLARPANEL_PUSHBACK :
        break;

    case SOLARPANEL_END :
        break;
    
    default:
        break;
    }

    if(loop<3 && step == 0){
        step = 4;
    }

    if(step == 0){
        returnValDeplacement = deplacementLinearPoint(robot,axeX,table[loop]-offsetRobotYellow1);
        ireturn = returnValDeplacement<0? returnValDeplacement : ireturn;
        if(returnValDeplacement){
            step++; 
        }
    }
    else if(step == 1){
        step++;
        startTime = millis() + 000;
    }
    else if(step == 2 && startTime < millis()){
        step++;
    }
    else if(step == 3){
        if(pullpush(arduino)){
            step++;
        }
    }
    else if(step == 4){
        returnValDeplacement = deplacementLinearPoint(robot,axeX,table[loop]-offsetRobotYellow1);
        ireturn = returnValDeplacement<0? returnValDeplacement : ireturn;
        if(returnValDeplacement){
            step++;
        }
    }
    else if(step == 5){
        step++;
        startTime = millis() + 000;
    }
    else if(step == 6 && startTime < millis()){
        step++;
    }
     else if(step == 7){
        if(pullpush(arduino)){
            loop++;
            step=0;
        }
    }

    initStat = false;
    if(nextState != currentState){
        int x,y,teta;
        robot->getCoords(x,y,teta);
        initStat = true;
    }
    currentState = nextState;
    return ireturn;

}

int takePlant(Asser* robot,Arduino* arduino,int yPos,int xStart, int xEnd, int numPlante){
    LOG_SCOPE("take plant");
    int ireturn = 0;
    const int plantexAxis[6] = {500,300,-300,-500,-300,300};
    static bool initStat = true;
    static fsmtakePlant_t currentState = TAKEPLANT_INIT;
    fsmtakePlant_t nextState = currentState;
    int deplacementreturn;
    static int positionToGo = 0;


    switch (currentState)
    {
    case TAKEPLANT_INIT :
        if(initStat) LOG_STATE("TAKEPLANT_INIT");
        nextState = TAKEPLANT_FORWARD;
        positionToGo = plantexAxis[numPlante]+300;
        break;
    case TAKEPLANT_FORWARD :
        if(initStat) LOG_STATE("TAKEPLANT_FORWARD");
        deplacementreturn = deplacementLinearPoint(robot,positionToGo,yPos);
        if(deplacementreturn>=1){
            nextState = TAKEPLANT_BACKWARD;
        }
        else if(deplacementreturn<=-1){
            nextState = TAKEPLANT_INIT;
            ireturn = -1;
        }
        break;
    case TAKEPLANT_REFORWARD : 
        if(initStat){ LOG_STATE("TAKEPLANT_REFORWARD");
            positionToGo += 400;
        }
        deplacementreturn = deplacementLinearPoint(robot,positionToGo,yPos);
        if(deplacementreturn>=1){
            nextState = TAKEPLANT_BACKWARD;
        }
        else if(deplacementreturn<=-1){
            nextState = TAKEPLANT_INIT;
            ireturn = -1;
        }
        break;
    case TAKEPLANT_BACKWARD :
        if(initStat){ LOG_STATE("TAKEPLANT_BACKWARD");
            positionToGo -= 100;
        }
        deplacementreturn = deplacementLinearPoint(robot,positionToGo,yPos);
        if(deplacementreturn>=1){
            nextState = TAKEPLANT_TAKE;
        }
        else if(deplacementreturn<=-1){
            nextState = TAKEPLANT_REFORWARD;
        }
        break;
    case TAKEPLANT_TAKE :
        if(initStat) LOG_STATE("TAKEPLANT_TAKE");
        deplacementreturn = catchPlant(arduino);
        if(deplacementreturn){
            nextState = TAKEPLANT_END;
        }
        break;
    case TAKEPLANT_END :
        if(initStat) LOG_STATE("TAKEPLANT_END");
        nextState = TAKEPLANT_INIT;
        ireturn = 1;
        break;
    
    default:
        if(initStat) LOG_STATE("default");
        nextState = TAKEPLANT_INIT;
        break;
    }

    initStat = false;
    if(nextState != currentState){
        initStat = true;
    }
    currentState = nextState;
    return ireturn;

}

int returnToHome(Asser* robot){
    static int step = 0;
    bool breturn = false;
    if(step == 0){
        robot->setLookForward(0,-1300,0);
        step++;   
    }
    else if(step == 1 && !robot->getError(ANGULAR_ERROR)){
        robot->linearSetpoint(0,-1300);
        step++;
    }
    else if(step == 2){
        if(!robot->getError(LINEAR_ERROR)){
            step++;
        }        
    }
    else if(step == 3){
        breturn = true;
    }

    return breturn; 
}

int FSMMatch(Asser* robot,Arduino* arduino){
    int  bFinMatch = turnSolarPannel(robot, arduino);
    if(bFinMatch == 1){
        printf("FIN turnSolarPannel\n");
    }
    // fsmMatch_t nextStateMatch = currentStateMatch;
    // switch (currentStateMatch)
    // {
    // case SOLARPANNEL:{
    //     bool bret = turnSolarPannel(robot, arduino,collideB);
    //     if(bret){
    //         nextStateMatch = RETURNHOME;
    //     }
    //     break;
    // }
    // case RETURNHOME:{
    //     int bFinMatch = returnToHome(robot,collideF);
    //     break;
    // }       
    
    // default:
    //     break;
    // }

    return bFinMatch;
}
