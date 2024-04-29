#pragma once

#include "robotState.h"
#include "tableState.hpp"

class robot
{
private:

public:
    robotState robotStatus;
    tableState tableStatus;
    /* data */
public:
    robot(/* args */);
    void robotloop(void);
    ~robot();
};


