#pragma once

#include "action.hpp"
#include "fonction.h"
#include <vector>

class actionContainer
{
private:
    action* takePlante0;
    action* takePlante1;
    action* takePlante2;
    action* takePlante3;
    action* takePlante4;
    action* takePlante5;

    action* putInJardiniere0;
    action* putInJardiniere1;
    action* putInJardiniere2;
    action* putInJardiniere3;
    action* putInJardiniere4;
    action* putInJardiniere5;

    action* currentAction;
    std::vector<action*> listeAction;

public:
    actionContainer(robotCDFR* imainRobot, Asser* irobot, Arduino* iarduino, tableState* itable);
    int actionContainerRun(void);
    void resetAllAction(void);
    ~actionContainer();
private : 

    int choosNextAction(void);
};

