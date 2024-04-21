#pragma once 

#include "asser.hpp"
#include "utils.h"
#include "config.h"
#include "arduino.hpp"

#define DELAYUPDOWN 1000


int pullpush(Arduino* arduino);
int catchPlant(Arduino* arduino);
int releasePlant(Arduino* arduino);

