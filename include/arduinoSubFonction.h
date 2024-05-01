#pragma once 

#include "asser.hpp"
#include "utils.h"
#include "config.h"
#include "arduino.hpp"

#define DELAYUPDOWN 1000

#define ELEVATORUP 2000
#define ELEVATORPLANT 700

#define CLAMPCLOSE 0
#define CLAMPOPEN 180
#define CLAMPSLEEP 15
#define CLAMPSTOP 20





int pullpush(Arduino* arduino);
int catchPlant(Arduino* arduino);
int releasePlant(Arduino* arduino);
void blinkLed(Arduino* arduino,int LedNb,int periode);

