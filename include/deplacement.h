#pragma once

#include "asser.hpp"
#include "utils.h"
#include "config.h"
#include "arduino.hpp"
#include "logger.hpp"

#define DISTANCESTOP   500
#define DISTANCERESTART 600

typedef enum {
    DEPLACEMENT_INIT,
    DEPLACEMENT_MOVE,
    DEPLACEMENT_STOP,
    DEPLACEMENT_WAIT
} deplcement_State_t;

int deplacementLinearPoint(Asser* robot, int x, int y);