#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <iostream>
#include <time.h>

#include "fonction.h"
#include "lidarAnalize.h"
#include "lidar.h"
#include "asser.hpp"
#include "arduino.hpp"
#include "utils.h"
#include "arduinoSubFonction.h"
#include "logger.hpp"

#define SIZEDATALIDAR 10000

typedef enum {
    INIT,
    INITIALIZE,
    SETHOME,
    WAITSTART,
    START,
    RUN,
    RETURNHOME,
    FIN,
    STOP
} main_State_t;

bool ctrl_c_pressed;
void ctrlc(int)
{
    ctrl_c_pressed = true;
}

int main() {
    LOG_INIT();


    if(!lidarSetup("/dev/ttyAMA0",256000)){
        return -1;
    }

    signal(SIGINT, ctrlc);
    signal(SIGTERM, ctrlc);

    Asser *robot = new Asser(I2C_ASSER_ADDR);
    LOG_SETROBOT(robot);
    lidarAnalize_t lidarData[SIZEDATALIDAR];    
    Arduino *arduino = new Arduino(100);
    main_State_t currentState = INIT;
    main_State_t nextState = INIT;
    unsigned long startTime;
    bool initStat; 
    colorTeam_t colorTeam;

    // arduino->servoPosition(1,180);
    // arduino->servoPosition(2,0);
    // arduino->moveStepper(2200,1);
    // while(!ctrl_c_pressed);
    // ctrl_c_pressed = false;
    // while(!catchPlant(arduino));
    // while(!ctrl_c_pressed);
    // ctrl_c_pressed = false;
    // while(!releasePlant(arduino));
    // while(!ctrl_c_pressed);

    while (1) {

        LOG_SCOPE("Main");
        
        int count = SIZEDATALIDAR;
        if(getlidarData(lidarData,count)){
            int x, y, teta;
            int distance;
            robot->getCoords(x,y,teta);
            position_t position = {x,y,teta,0};
            convertAngularToAxial(lidarData,count,position);
            //pixelArtPrint(lidarData,count,50,50,100,position);
            robot->getBrakingDistance(distance);
            robot->collide = collide(lidarData,count,distance);
            //printf("distance : %d \t collide : %d\n",distance,robot->collide);

        }

        switch (currentState) {
            //****************************************************************
            case INIT:
                if(initStat) printf("=> STATE : INIT\n");
                nextState = INITIALIZE;
                break;
            //****************************************************************
            case INITIALIZE:{
                if(initStat) printf("=> STATE : INITIALIZE\n");
                if(initStat){
                    int bStateCapteur2;
                    arduino->readCapteur(2,bStateCapteur2);
                    colorTeam = bStateCapteur2 ? BLUE : YELLOW;
                    if(colorTeam == YELLOW){
                        //robot->setCoords(800,1250,-90);
                        robot->setCoords(-1000,0,0);
                        printf("teams : YELLOW\n");
                    }
                    else{
                        robot->setCoords(830,-1440,-90);
                        printf("teams : BLUE\n");
                    }
                    robot->enableMotor(true);
                    arduino->servoPosition(1,180);
                    arduino->servoPosition(2,0);
                    arduino->moveStepper(2200,1);
                }
                //robot->setCoords(0,1500,-90);   
                //robot->linearSetpoint(0,1400);
                nextState = SETHOME;
                break;
            }
            //****************************************************************
            case SETHOME:{
                if(initStat) printf("=> STATE : SETHOME\n");
                nextState = WAITSTART;
                // if(initPositon(robot,800,1250,-90)){
                //     nextState = WAITSTART;
                // }
                break;
            }            
            case WAITSTART:{
                if(initStat) printf("=> STATE : WAITSTART\n");
                int bStateCapteur1;
                arduino->readCapteur(1,bStateCapteur1);
                if(bStateCapteur1 == 0 && robot->collide > 500){
                    nextState = START;
                }
                break;
            }
            //****************************************************************      
            case START:
                if(initStat) printf("=> STATE : START\n");
                startTime = millis();
                nextState = RUN;
                break;
            //****************************************************************
            case RUN:{
                if(initStat) printf("=> STATE : RUN\n");
                bool finish = takePlant(robot, arduino,0,-1000,0,3);
                //bool finish =  FSMMatch(robot, arduino);
                if(startTime+80000 < millis() || finish){
                    nextState = FIN;
                }
                break;
            }
            case RETURNHOME:{
                if(initStat) printf("=> STATE : RETURNHOME\n");
                bool finish =  returnToHome(robot);
                if(startTime+90000 < millis() || finish){
                    nextState = FIN;
                }
                break;
            }
            //****************************************************************
            case FIN:
                if(initStat) printf("=> STATE : FIN\n");
                //arduino->servoPosition(2,180);
                nextState = STOP;
                break;
            //****************************************************************
            case STOP:
                if(initStat) printf("=> STATE : STOP\n");
                nextState = STOP;
                break;
            //****************************************************************
            default:
                printf("=> STATE : non reconize event in main FSM\n");
                nextState = STOP;
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

    arduino->servoPosition(1,180);
    arduino->servoPosition(2,0);
    arduino->moveStepper(0,1);
    robot->enableMotor(false);
    robot->stop();
    lidarStop();

    return 0;
}