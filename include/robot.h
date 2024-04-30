#pragma once

#include "robotState.h"
#include "tableState.hpp"

class robotCDFR
{
private:

public:
    robotState robotStatus;
    tableState tableStatus;
    /* data */
public:
    robotCDFR(/* args */);
    void robotCDFRloop(void);
    ~robotCDFR();
};


