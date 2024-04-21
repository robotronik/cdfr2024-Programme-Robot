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



int turnSolarPannel(Asser* robot,Arduino* arduino,int collide){
    static unsigned long startTime;
    static int step = 0;
    static int loop = 0;
    const int offsetRobotYellow1 = 5;
    const int offsetRobotYellow2 = 15;
    const int table[9] = {1225,1000,775,225,0,-225,-775,-1000,-1225};
    const int axeX = 800;
    //const int table[6] = {-50,-275,-400,-900,-1125,-1350};

    if(loop<3 && step == 0){
        step = 4;
    }

    if(step == 0){
        robot->linearSetpoint(axeX,table[loop]-offsetRobotYellow1);
        step++;   
    }
    else if(step == 1 && !robot->getError(LINEAR_ERROR)){
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
        robot->linearSetpoint(axeX,table[loop]-offsetRobotYellow2);
        step++;
    }
    else if(step == 5 && !robot->getError(LINEAR_ERROR)){
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

    bool breturn = false;
    if(loop == 6){
        loop = 0;
        breturn = true;
    }
    if(collide){
        printf("collide!!!!\n");
        breturn = true;
    }
    return breturn;

}

int returnToHome(Asser* robot,int collide){
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
        if(collide){
            breturn = true;
        }
        if(!robot->getError(LINEAR_ERROR)){
            step++;
        }        
    }
    else if(step == 3){
        breturn = true;
    }

    return breturn; 
}

int FSMMatch(Asser* robot,Arduino* arduino, int collideF, int collideB){
    int  bFinMatch = turnSolarPannel(robot, arduino,collideB);
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
