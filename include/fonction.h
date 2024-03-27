#ifndef MYFUNCTION_H
#define MYFUNCTION_H

#include "asser.hpp"
#include "utils.h"
#include "config.h"
#include "arduino.hpp"

int initPositon(Asser* robot);
int turnSolarPannel(Asser* robot,Arduino* arduino);
int pullpush(Arduino* arduino);

#endif // MYFUNCTION_H
