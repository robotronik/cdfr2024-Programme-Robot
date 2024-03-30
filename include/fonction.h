#ifndef MYFUNCTION_H
#define MYFUNCTION_H

#include "asser.hpp"
#include "utils.h"
#include "config.h"
#include "arduino.hpp"

typedef enum {
    SOLARPANNEL
} fsmMatch_t;

int initPositon(Asser* robot);
int turnSolarPannel(Asser* robot,Arduino* arduino, int collide);
int returnToHome(Asser* robot, int collide);
int pullpush(Arduino* arduino);
int FSMMatch(Asser* robot,Arduino* arduino, int collideF, int collideB);

#endif // MYFUNCTION_H
