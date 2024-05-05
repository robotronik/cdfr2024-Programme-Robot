#include "actionContainer.hpp"
#include "logger.hpp"

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
    takePlante0->setStartPoint(itable->plantPosition[0].x - 400,itable->plantPosition[0].y,0,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante0->setRunAction([&](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant(*iRobot,iAsser,iarduino,itable,itable->plantPosition[0].y,itable->plantPosition[0].x - 400,itable->plantPosition[0].x + 400,0);
    });
    takePlante0->goodEnd([](tableState*itable){
        itable->robotHavePlante = true;
        itable->planteStockFull[0] = false;
    });
    takePlante0->setCostAction([](tableState*itable){
        return itable->planteStockFull[0] && !itable->robotHavePlante ? -1 : -1;
    });
    listeAction.push_back(takePlante0);

// ACTION
    takePlante1->setStartPoint(itable->plantPosition[1].x - 400,itable->plantPosition[1].y,0,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante1->setRunAction([&](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant(*iRobot,iAsser,iarduino,itable,itable->plantPosition[1].y,itable->plantPosition[1].x - 400,itable->plantPosition[1].x + 400,1);
    });
    takePlante1->goodEnd([](tableState*itable){
        itable->robotHavePlante = true;
        itable->planteStockFull[1] = false;
    });
    takePlante1->setCostAction([](tableState*itable){
        return itable->planteStockFull[1] && !itable->robotHavePlante ? 90 : -1;
    });
    listeAction.push_back(takePlante1);

// ACTION
    takePlante2->setStartPoint(itable->plantPosition[2].x - 400,itable->plantPosition[2].y,0,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante2->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant(*iRobot,iAsser,iarduino,itable,itable->plantPosition[2].y,itable->plantPosition[2].x - 400,itable->plantPosition[2].x + 400,2);
    });
    takePlante2->goodEnd([](tableState*itable){
        itable->robotHavePlante = true;
        itable->planteStockFull[2] = false;
    });
    takePlante2->setCostAction([](tableState*itable){
        return itable->planteStockFull[2] && !itable->robotHavePlante ? 100 : -1;
    });
    listeAction.push_back(takePlante2);

// ACTION
    takePlante3->setStartPoint(itable->plantPosition[3].x - 400,itable->plantPosition[3].y,0,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante3->setRunAction([&](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant(*iRobot,iAsser,iarduino,itable,itable->plantPosition[3].y,itable->plantPosition[3].x - 400,itable->plantPosition[3].x + 400,3);
    });
    takePlante3->goodEnd([](tableState*itable){
        itable->robotHavePlante = true;
        itable->planteStockFull[3] = false;
    });
    takePlante3->setCostAction([](tableState*itable){
        return itable->planteStockFull[3] && !itable->robotHavePlante ? -1 : -1;
    });
    listeAction.push_back(takePlante3);

// ACTION
    takePlante4->setStartPoint(itable->plantPosition[4].x - 400,itable->plantPosition[4].y,0,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante4->setRunAction([&](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant(*iRobot,iAsser,iarduino,itable,itable->plantPosition[4].y,itable->plantPosition[4].x - 400,itable->plantPosition[4].x + 400,4);
    });
    takePlante4->goodEnd([](tableState*itable){
        itable->robotHavePlante = true;
        itable->planteStockFull[4] = false;
    });
    takePlante4->setCostAction([](tableState*itable){
        return itable->planteStockFull[4] && !itable->robotHavePlante ? 80 : -1;
    });
    listeAction.push_back(takePlante4);

// ACTION
    takePlante5->setStartPoint(itable->plantPosition[5].x - 400,itable->plantPosition[5].y,0,MOVE_FORWARD,ROTATION_DIRECT);
    takePlante5->setRunAction([&](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return takePlant(*iRobot,iAsser,iarduino,itable,itable->plantPosition[5].y,itable->plantPosition[5].x - 400,itable->plantPosition[5].x + 400,5);
    });
    takePlante5->goodEnd([](tableState*itable){
        itable->robotHavePlante = true;
        itable->planteStockFull[5] = false;
    });
    takePlante5->setCostAction([](tableState*itable){
        return itable->planteStockFull[5] && !itable->robotHavePlante ? 70 : -1;
    });
    listeAction.push_back(takePlante5);





// ACTION
    putInJardiniere3->setStartPoint(-700,-762,180,MOVE_FORWARD,ROTATION_DIRECT);
    putInJardiniere3->setEndPoint(-700,-762,180,MOVE_BACKWARD,ROTATION_DIRECT);
    putInJardiniere3->setRunAction([](action* iaction, robotCDFR* iRobot, Asser* iAsser, Arduino* iarduino, tableState*itable) {
        return jardinierePutPlant(*iRobot,iAsser,iarduino,-870,-762,-180);
    });
    putInJardiniere3->goodEnd([](tableState*itable){
        itable->robotHavePlante = false;
        //itable->JardiniereFull[2] = true;
    });
    putInJardiniere3->setCostAction([](tableState*itable){
        return !itable->JardiniereFull[2] && itable->robotHavePlante ? 99 : -1;
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
    LOG_GREEN_INFO("CHOOSE NEW ACTION: ");
    int bestCost = -1;
    for (action* elem : listeAction) {
        int cost = elem->costAction();
        if(cost>bestCost){
            bestCost = cost;
            currentAction = elem;
        }
    }
    if(bestCost!=-1)LOG_GREEN_INFO("CHOOSE : ",currentAction->getName());
    return bestCost;
}

void actionContainer::resetAllAction(void){
    for (action* elem : listeAction) {
        elem->resetActionEnable();
    }
}