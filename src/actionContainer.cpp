#include "actionContainer.hpp"




actionContainer::actionContainer(robotCDFR* imainRobot, Asser* irobot, Arduino* iarduino, tableState* itable){
    
    takePlante0 = new action(imainRobot,irobot,iarduino,itable);
    takePlante1 = new action(imainRobot,irobot,iarduino,itable);
    takePlante2 = new action(imainRobot,irobot,iarduino,itable);
    takePlante3 = new action(imainRobot,irobot,iarduino,itable);
    takePlante4 = new action(imainRobot,irobot,iarduino,itable);
    takePlante5 = new action(imainRobot,irobot,iarduino,itable);

    takePlante2->setStartPoint(-700,-505,0,MOVE_FORWARD,ROTATION_DIRECT);

    takePlante2->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant(*iRobot,iAsser,iarduino,-505,-700,0,2);
    });
}

int actionContainer::actionContainerRun(void){
    return takePlante2->runAction();
}

actionContainer::~actionContainer(){
}