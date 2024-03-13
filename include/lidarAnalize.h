#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "config.h"
#include "math.h"

#define MAP(value, fromLow, fromHigh, toLow, toHigh) ((toLow) + (((value) - (fromLow)) * ((toHigh) - (toLow)) / ((fromHigh) - (fromLow))))



void convertAngularToAxial(lidarAnalize_t* data, int count, position_t position);

bool collideFordward(lidarAnalize_t* data, int count, int distanceStop);

bool collideBackward(lidarAnalize_t* data, int count ,int distanceStop);

void printLidarAxial(lidarAnalize_t* data, int count);

void printAngular(lidarAnalize_t* data, int count);

void pixelArtPrint(lidarAnalize_t* data, int count,int sizeX,int sizeY,int scale,position_t position);
