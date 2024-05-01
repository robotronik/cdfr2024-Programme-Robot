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



int turnSolarPannel(robotCDFR mainRobot, Asser* robot,Arduino* arduino){
    LOG_SCOPE("SolarPanel");
    static fsmSolarPanel_t currentState = SOLARPANEL_INIT;
    fsmSolarPanel_t nextState = currentState;
    static bool initStat = true;
    int ireturn = 0;
    int deplacementreturn;
    const int axeX = 800;
    static int solarPanelNumber;
    int offsetRobot1;
    int offsetRobot2;
    const int table[9] = {1225,1000,775,225,0,-225,-775,-1000,-1225};

    if(mainRobot.robotStatus.colorTeam == YELLOW){
        offsetRobot1 = 5;
        offsetRobot2 = -10;
    }
    else{
        offsetRobot1 = -5;
        offsetRobot2 = 10;
    }
    
    switch (currentState)
    {
    case SOLARPANEL_INIT :
        if(initStat) LOG_STATE("SOLARPANEL_INIT");
        nextState = SOLARPANEL_FORWARD;
        if(mainRobot.robotStatus.colorTeam == YELLOW){
            solarPanelNumber = 0;
        }
        else{
            solarPanelNumber = 8;
        }
        break;

    case SOLARPANEL_FORWARD :
        if(initStat) LOG_STATE("SOLARPANEL_FORWARD");
        deplacementreturn = deplacementLinearPoint(mainRobot,robot,axeX,table[solarPanelNumber]-offsetRobot1);
        if(deplacementreturn>0){
            nextState = SOLARPANEL_PUSHFOR; 
        }
        else if(deplacementreturn<0){
            ireturn = -1;
            nextState = SOLARPANEL_INIT;
        }
        break;

    case SOLARPANEL_PUSHFOR :
        if(initStat) LOG_STATE("SOLARPANEL_PUSHFOR");
        if(pullpush(arduino)){
            if(mainRobot.robotStatus.colorTeam == YELLOW){
                if(solarPanelNumber<3){
                    solarPanelNumber++;
                    nextState = SOLARPANEL_FORWARD;
                }
                else{
                    nextState = SOLARPANEL_BACKWARD;
                }
            }
            else{
                if(solarPanelNumber>5){
                    solarPanelNumber--;
                    nextState = SOLARPANEL_FORWARD;
                }
                else{
                    nextState = SOLARPANEL_BACKWARD;
                }
            }
        }
        break;

    case SOLARPANEL_BACKWARD :
        if(initStat) LOG_STATE("SOLARPANEL_BACKWARD");
        deplacementreturn = deplacementLinearPoint(mainRobot,robot,axeX,table[solarPanelNumber]-offsetRobot2);
        if(deplacementreturn>0){
            nextState = SOLARPANEL_PUSHBACK;
        }
        else if(deplacementreturn<0){
            ireturn = -1;
            nextState = SOLARPANEL_INIT;
        }
        break;

    case SOLARPANEL_PUSHBACK :
        if(initStat) LOG_STATE("SOLARPANEL_PUSHBACK");
        if(pullpush(arduino)){
            if(mainRobot.robotStatus.colorTeam == YELLOW){
                if(solarPanelNumber==5){
                    nextState = SOLARPANEL_END;
                }
                else{
                    solarPanelNumber++;
                    nextState = SOLARPANEL_FORWARD;
                }
            }
            else{
                solarPanelNumber--;
                if(solarPanelNumber==3){
                    nextState = SOLARPANEL_END;
                }
                else{
                    nextState = SOLARPANEL_FORWARD;
                }
            }
        }
        break;

    case SOLARPANEL_END :
        if(initStat) LOG_STATE("SOLARPANEL_END");
        nextState = SOLARPANEL_INIT;
        ireturn = 1;
        break;
    
    default:
        if(initStat) LOG_ERROR("SOLARPANEL_DEFAULT");
        break;
    }

    initStat = false;
    if(nextState != currentState){
        initStat = true;
    }
    currentState = nextState;
    return ireturn;

}

int takePlant(robotCDFR mainRobot, Asser* robot,Arduino* arduino,int yPos,int xStart, int xEnd, int numPlante){
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
        robot->setLinearMaxSpeed(100);
        break;
    case TAKEPLANT_FORWARD :
        if(initStat) LOG_STATE("TAKEPLANT_FORWARD");
        deplacementreturn = deplacementLinearPoint(mainRobot,robot,positionToGo,yPos);
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
        deplacementreturn = deplacementLinearPoint(mainRobot,robot,positionToGo,yPos);
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
            positionToGo -= 125;
        }
        deplacementreturn = deplacementLinearPoint(mainRobot,robot,positionToGo,yPos);
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
        robot->setLinearMaxSpeed(10000);
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

int FSMMatch(robotCDFR mainRobot, Asser* robot,Arduino* arduino){
    int  bFinMatch = turnSolarPannel(mainRobot,robot, arduino);
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

int TestPinceFSM(robotCDFR mainRobot, Asser* robot,Arduino* arduino){
    LOG_SCOPE("TestPince");
    int ireturn = 0;
    static bool initStat = true;
    static fsmTestPince_t currentState = TESTPINCE_INIT;
    fsmTestPince_t nextState = currentState;
    int deplacementreturn;


    switch (currentState)
    {
    //****************************************************************
    case TESTPINCE_INIT :
        if(initStat) LOG_STATE("TESTPINCE_INIT");
        nextState = TESTPINCE_GOPLANT;
        break;
    //****************************************************************
    case TESTPINCE_GOPLANT :
        if(initStat) LOG_STATE("TESTPINCE_GOPLANT");
        deplacementreturn = deplacementgoToPoint(mainRobot,robot,-500,-500,0,MOVE_FORWARD,ROTATION_DIRECT);
        if(deplacementreturn>0){
            nextState = TESTPINCE_TAKEPLANT;
        }
        else if(deplacementreturn<0){
            return deplacementreturn;
            nextState = TESTPINCE_INIT;
        }
        break;
    //****************************************************************        
    case TESTPINCE_TAKEPLANT :
        if(initStat) LOG_STATE("TESTPINCE_TAKEPLANT");
        deplacementreturn = takePlant(mainRobot,robot,arduino,-500,-500,0,2);
        if(deplacementreturn>0){
            nextState = TESTPINCE_GOCORNE;
            ireturn = 1; // END HERE FOR TODAY
        }
        else if(deplacementreturn<0){
            return deplacementreturn;
            nextState = TESTPINCE_INIT;
        }
        break;
    //****************************************************************        
    case TESTPINCE_GOCORNE :
        if(initStat) LOG_STATE("TESTPINCE_GOCORNE");
        deplacementreturn = deplacementgoToPoint(mainRobot,robot,-700,-762,0,MOVE_FORWARD,ROTATION_DIRECT);
        if(deplacementreturn>0){
            nextState = TESTPINCE_GOJARDINIER;
        }
        else if(deplacementreturn<0){
            return deplacementreturn;
            nextState = TESTPINCE_INIT;
        }
        break;
    //****************************************************************
    case TESTPINCE_GOJARDINIER :
        if(initStat) LOG_STATE("TESTPINCE_GOJARDINIER");
        deplacementreturn = deplacementgoToPoint(mainRobot,robot,-860,-762,0,MOVE_FORWARD,ROTATION_DIRECT);
        if(deplacementreturn>0){
            nextState = TESTPINCE_PLACE;
        }
        else if(deplacementreturn<0){
            return deplacementreturn;
            nextState = TESTPINCE_INIT;
        }
        break;
    //****************************************************************
    case TESTPINCE_PLACE :
        if(initStat) LOG_STATE("TESTPINCE_PLACE");
        if(releasePlant(arduino)){
            nextState = TESTPINCE_GOBACKWARD;
        }
        break;
    //****************************************************************
    case TESTPINCE_GOBACKWARD :
        if(initStat) LOG_STATE("TESTPINCE_GOBACKWARD");
        deplacementreturn = deplacementgoToPoint(mainRobot,robot,-800,-1250,-90,MOVE_BACKWARD,ROTATION_DIRECT);
        if(deplacementreturn>0){
            nextState = TESTPINCE_INIT;
            ireturn = 1;
        }
        else if(deplacementreturn<0){
            return deplacementreturn;
            nextState = TESTPINCE_INIT;
        }
        break;
    //****************************************************************    
    default:
        if(initStat) LOG_ERROR("default");
        nextState = TESTPINCE_INIT;
        break;
    }

    initStat = false;
    if(nextState != currentState){
        initStat = true;
    }
    currentState = nextState;
    return ireturn;
}
