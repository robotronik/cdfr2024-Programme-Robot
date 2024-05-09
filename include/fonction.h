#ifndef MYFUNCTION_H
#define MYFUNCTION_H

#include "asser.hpp"
#include "utils.h"
#include "config.h"
#include "arduino.hpp"
#include "deplacement.h"
#include "arduinoSubFonction.h"
#include "logger.hpp"
#include "robot.h"


typedef enum {
    TAKEPLANT_INIT,
    TAKEPLANT_FORWARD,
    TAKEPLANT_REFORWARD,
    TAKEPLANT_BACKWARD,
    TAKEPLANT_TAKE,
    TAKEPLANT_END
} fsmtakePlant_t;

typedef enum {
    SOLARPANEL_INIT,
    SOLARPANEL_SETHOME,
    SOLARPANEL_FORWARD,
    SOLARPANEL_PUSHFOR,
    SOLARPANEL_BACKWARD,
    SOLARPANEL_PUSHBACK,
    SOLARPANEL_END
} fsmSolarPanel_t;

typedef enum {
    TESTPINCE_INIT,
    TESTPINCE_GOPLANT,
    TESTPINCE_TAKEPLANT,
    TESTPINCE_GOCORNE,
    TESTPINCE_GOJARDINIER,
    TESTPINCE_PLACE,
    TESTPINCE_GOBACKWARD,
    TESTPINCE_GOHOME
} fsmTestPince_t;

typedef enum {
    PUTPLANT_INIT,
    PUTPLANT_GOBORDER,
    PUTPLANT_PLACE,
} fsmTPutPlant_t;

typedef enum {
    INTIY_INIT,
    INTIY_BACKWARD
} fsminitY_t;


typedef enum {
    SETPOS_INIT,
    SETPOS_WAITINIT,
    SETPOS_FIRSTBACKWARD,
    SETPOS_FIRSTFORWARD,
    SETPOS_SECONDBACKWARD,
    SETPOS_SECONDFORWARD
} fsminitPos_t;



int initPositon(Asser* robot,int x, int y,int teta);
int initPositon2(robotCDFR mainRobot, Asser* robot,int x, int y,int teta);
int initY(robotCDFR mainRobot, Asser* robot,int x, int y,int teta);
int turnSolarPannel(robotCDFR mainRobot,Asser* robot,Arduino* arduino);
int takePlant(robotCDFR mainRobot,Asser* robot,Arduino* arduino,tableState*itable,int yPos,int xStart, int xEnd, int numPlante);
int returnToHome(Asser* robot);
int jardinierePutPlant(robotCDFR mainRobot, Asser* robot,Arduino* arduino,int x,int y,int teta);
bool allJardiniereFull(tableState* itable);
void resetActionneur(Asser* robot, Arduino* arduino);
int FSMMatch(robotCDFR mainRobot,Asser* robot,Arduino* arduino);
int TestPinceFSM(robotCDFR mainRobot, Asser* robot,Arduino* arduino);

#endif // MYFUNCTION_H
