#include "tableState.hpp"


tableState::tableState(/* args */){
    for(int i = 0; i<6;i++){
        planteStockFull[i] = true;
        JardiniereFull[i] = false;
        zoneFull[i] = false;
    }
    for(int i =0; i < 9; i++){
        panneauSolaireRotate[i] = NONE;        
    }
        
    robotHavePlante = false;
}

tableState::~tableState(){
}
