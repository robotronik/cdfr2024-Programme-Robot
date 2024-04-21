#ifndef MYFUNCTION_H
#define MYFUNCTION_H

#include "asser.hpp"
#include "utils.h"
#include "config.h"
#include "arduino.hpp"
#include "deplacement.h"

typedef enum {
    SOLARPANNEL
} fsmMatch_t;

int initPositon(Asser* robot,int x, int y,int teta);
int turnSolarPannel(Asser* robot,Arduino* arduino);
int returnToHome(Asser* robot);
int pullpush(Arduino* arduino);
int FSMMatch(Asser* robot,Arduino* arduino);

#endif // MYFUNCTION_H
