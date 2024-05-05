#pragma once

#include "action.hpp"
#include "fonction.h"

class actionContainer
{
private:
    action* takePlante0;
    action* takePlante1;
    action* takePlante2;
    action* takePlante3;
    action* takePlante4;
    action* takePlante5;
public:
    actionContainer(robotCDFR* imainRobot, Asser* irobot, Arduino* iarduino, tableState* itable);
    int actionContainerRun(void);
    ~actionContainer();
};

