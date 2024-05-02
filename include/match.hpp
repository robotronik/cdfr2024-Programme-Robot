#pragma once


#include "asser.hpp"
#include "utils.h"
#include "config.h"
#include "arduino.hpp"
#include "deplacement.h"
#include "arduinoSubFonction.h"
#include "logger.hpp"
#include "robot.h"
#include "fonction.h"



typedef enum {
    MATCH_INIT,
    MATCH_PLANT,
    MATCH_SOLARPANEL,
    MATCH_GOHOME1,
    MATCH_GOHOME2,
    MATCH_END
} fsmMatch_t;