#include "actionContainer.hpp"


actionContainer::actionContainer(robotCDFR* imainRobot, Asser* irobot, Arduino* iarduino, tableState* itable){
    
    takePlante0 = new action("takePlante0",imainRobot,irobot,iarduino,itable);
    takePlante1 = new action("takePlante1",imainRobot,irobot,iarduino,itable);
    takePlante2 = new action("takePlante2",imainRobot,irobot,iarduino,itable);
    takePlante3 = new action("takePlante3",imainRobot,irobot,iarduino,itable);
    takePlante4 = new action("takePlante4",imainRobot,irobot,iarduino,itable);
    takePlante5 = new action("takePlante5",imainRobot,irobot,iarduino,itable);

    putInJardiniere0 = new action("putInJardiniere0",imainRobot,irobot,iarduino,itable);
    putInJardiniere1 = new action("putInJardiniere1",imainRobot,irobot,iarduino,itable);
    putInJardiniere2 = new action("putInJardiniere2",imainRobot,irobot,iarduino,itable);
    putInJardiniere3 = new action("putInJardiniere3",imainRobot,irobot,iarduino,itable);
    putInJardiniere4 = new action("putInJardiniere4",imainRobot,irobot,iarduino,itable);
    putInJardiniere5 = new action("putInJardiniere5",imainRobot,irobot,iarduino,itable);


// ACTION
    takePlante2->setStartPoint(-700,-505,0,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante2->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        int iRet = takePlant(*iRobot,iAsser,iarduino,-505,-700,0,2);
        if(iRet>0){
            itable->robotHavePlante = true;
            itable->planteStockFull[2] = false;
        }
        return iRet;
    });
    takePlante2->setCostAction([](tableState*itable){
        return itable->planteStockFull[2] ? 100 : -1;
    });
    listeAction.push_back(takePlante2);

// ACTION
    putInJardiniere3->setStartPoint(-700,-762,180,MOVE_FORWARD,ROTATION_DIRECT);
    putInJardiniere3->setEndPoint(-700,-762,180,MOVE_BACKWARD,ROTATION_DIRECT);
    putInJardiniere3->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        int iRet = jardinierePutPlant(*iRobot,iAsser,iarduino,-870,-762,-180);
        if(iRet>0){
            itable->robotHavePlante = false;
            itable->JardiniereFull[2] = true;
        }
        return iRet;
    });
    putInJardiniere3->setCostAction([](tableState*itable){
        return itable->JardiniereFull[2] ? -1 : 99;
    });
    listeAction.push_back(putInJardiniere3);



    //Choose first action
    choosNextAction();
}

int actionContainer::actionContainerRun(void){
    int iActionReturn;
    int iChoosNextReturn = 0;
    int iRet = 0;
    iActionReturn = currentAction->runAction();
    if(iActionReturn>0){
        iChoosNextReturn = choosNextAction();
    }
    else if(iActionReturn<0){
        iChoosNextReturn = choosNextAction();
    }
    if(iChoosNextReturn == -1){
        iRet = -1;
    }
    return iRet;
}

actionContainer::~actionContainer(){
}


int actionContainer::choosNextAction(void){
    int bestCost = -1;
    for (action* elem : listeAction) {
        if(elem->costAction()>bestCost){
            currentAction = elem;
        }
    }
    LOG_GREEN_INFO("CHOOSE : ",currentAction->getName());
    return bestCost;
}
