#pragma once

#include "asser.hpp"
#include "utils.h"
#include "config.h"
#include "arduino.hpp"
#include "logger.hpp"
#include "robot.h"

#define DISTANCESTOP   500
#define DISTANCERESTART 600

typedef enum {
    DEPLACEMENT_INIT,
    DEPLACEMENT_WAITFIRSTMOVE,
    DEPLACEMENT_MOVE,
    DEPLACEMENT_STOP,
    DEPLACEMENT_WAIT
} deplcement_State_t;

typedef enum {
    GOTO_INIT,
    GOTO_LOOKAT,
    GOTO_MOVE,
    GOTO_TURN,
} go_to_State_t;

int deplacementLinearPoint(robotCDFR mainRobot,Asser* robot, int x, int y);
int deplacementgoToPoint(robotCDFR mainRobot, Asser* robot, int x, int y, int teta, int direction,int rotation);