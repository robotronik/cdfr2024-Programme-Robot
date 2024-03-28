#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <iostream>

#include "fonction.h"
#include "lidarAnalize.h"
#include "lidar.h"
#include "asser.hpp"
#include "arduino.hpp"
#include "utils.h"

#define SIZEDATALIDAR 10000

typedef enum {
    INIT,
    INITIALIZE,
    WAITSTART,
    START,
    RUN,
    FIN,
    STOP
} main_State_t;

bool ctrl_c_pressed;
void ctrlc(int)
{
    ctrl_c_pressed = true;
}

int main() {

    if(!lidarSetup("/dev/ttyAMA0",256000)){
        return -1;
    }

    signal(SIGINT, ctrlc);
    signal(SIGTERM, ctrlc);

    lidarAnalize_t lidarData[SIZEDATALIDAR];
    bool b_collidefordward;
    bool b_collideBackward;
    Asser *robot = new Asser(I2C_ASSER_ADDR);
    Arduino *arduino = new Arduino(100);
    main_State_t currentState = INIT;
    main_State_t nextState = INIT;
    unsigned long startTime;
    bool initStat; 
    colorTeam_t colorTeam; 


    while (1) {
        
        int count = SIZEDATALIDAR;
        if(getlidarData(lidarData,count)){
            int x, y, teta;
            robot->getCoords(x,y,teta);
            position_t position = {x,y,teta,0};
            convertAngularToAxial(lidarData,count,position);
            //pixelArtPrint(lidarData,count,50,50,100,position);
            b_collidefordward = collideFordward(lidarData,count);
            b_collideBackward = collideBackward(lidarData,count);
        }

        switch (currentState) {
            //****************************************************************
            case INIT:
                nextState = INITIALIZE;
                if(initStat) printf("STATE : INIT\n");
                break;
            //****************************************************************
            case INITIALIZE:{
                if(initStat){
                    int bStateCapteur2;
                    arduino->readCapteur(2,bStateCapteur2);
                    colorTeam = bStateCapteur2 ? YELLOW : BLUE;
                }
                if(colorTeam == YELLOW){
                    //robot->setCoords(1365,830,-90);
                }
                else{
                    //robot->setCoords(-1440,830,-90);
                }            
                robot->enableMotor(true);
                robot->setCoords(1000,0,0);
                robot->linearSetpoint(900,0);
                nextState = WAITSTART;
                if(initStat) printf("STATE : INITIALIZE\n");
                break;
            }
            //****************************************************************
            case WAITSTART:{
                int bStateCapteur1;
                arduino->readCapteur(1,bStateCapteur1);
                if(bStateCapteur1 == 0){
                    nextState = START;
                }
                if(initStat) printf("STATE : WAITSTART\n");
                break;
            }
            //****************************************************************      
            case START:
                startTime = millis()+90000;
                nextState = RUN;
                if(initStat) printf("STATE : START\n");
                break;
            //****************************************************************
            case RUN:{
                bool finish = false ;turnSolarPannel(robot, arduino);
                if(startTime < millis() || finish){
                    nextState = FIN;
                }
                if(initStat) printf("STATE : RUN\n");
                break;
            }
            //****************************************************************
            case FIN:
                nextState = STOP;
                if(initStat) printf("STATE : FIN\n");
                break;
            //****************************************************************
            case STOP:
                nextState = STOP;
                if(initStat) printf("STATE : STOP\n");
                break;
            //****************************************************************
            default:
                nextState = STOP;
                printf("non reconize event in main FSM\n");
                break;
        }

        initStat = false;
        if(currentState != nextState){
            initStat = true;
        }
        currentState = nextState;


        //ctrl_c_pressed |= turnSolarPannel(robot,arduino);

        if (ctrl_c_pressed){ 
            break;
        }
    }




    robot->enableMotor(false);
    robot->stop();
    lidarStop();

    return 0;
}