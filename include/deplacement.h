#pragma once

#include "asser.hpp"
#include "utils.h"
#include "config.h"
#include "arduino.hpp"

#define DISTANCESTOP   500
#define DISTANCERESTART 750

typedef enum {
    DEPLACEMENT_INIT,
    DEPLACEMENT_MOVE,
    DEPLACEMENT_STOP,
    DEPLACEMENT_WAIT
} deplcement_State_t;

int deplacementLinearPoint(Asser* robot, int x, int y);