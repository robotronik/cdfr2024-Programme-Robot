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

    printf("  _____   ____  ____   ____ _______ _____   ____  _   _ _____ _  __\n");
    printf(" |  __ \\ / __ \\|  _ \\ / __ \\__   __|  __ \\ / __ \\| \\ | |_   _| |/ /\n");
    printf(" | |__) | |  | | |_) | |  | | | |  | |__) | |  | |  \\| | | | | ' / \n");
    printf(" |  _  /| |  | |  _ <| |  | | | |  |  _  /| |  | | . ` | | | |  <  \n");
    printf(" | | \\ \\| |__| | |_) | |__| | | |  | | \\ \\| |__| | |\\  |_| |_| . \\ \n");
    printf(" |_|  \\_\\\\____/|____/ \\____/  |_|  |_|  \\\\_\\____/|_| \\_|_____|_|\\_\\\n\n");                                                                 
                                                                   
    printf("ROBOTRONIK\n");
    printf("PROGRAM ROBOT CDFR\n");
    time_t temps;
    struct tm date;
    char tempsFormate[80];
    time(&temps);
    date = *localtime(&temps);
    strftime(tempsFormate, sizeof(tempsFormate), "%Y-%m-%d %H:%M:%S", &date);
    printf("Start Time : %s\n", tempsFormate);


    if(!lidarSetup("/dev/ttyAMA0",256000)){
        return -1;
    }

    signal(SIGINT, ctrlc);
    signal(SIGTERM, ctrlc);

    lidarAnalize_t lidarData[SIZEDATALIDAR];
    Asser *robot = new Asser(I2C_ASSER_ADDR);
    Arduino *arduino = new Arduino(100);
    main_State_t currentState = INIT;
    main_State_t nextState = INIT;
    unsigned long startTime;
    bool initStat; 
    colorTeam_t colorTeam;

    // while (1) {
    //     while(!releasePlant(arduino));
    //     sleep(2);
    //     while(!catchPlant(arduino));
    //     sleep(2);

    //     if (ctrl_c_pressed){ 
    //         break;
    //     }
    // }

    while (1) {
        
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
                        robot->setCoords(800,1250,-90);
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
                //bool finish = turnSolarPannel(robot, arduino,b_collideBackward);
                bool finish =  FSMMatch(robot, arduino);
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
                arduino->servoPosition(2,180);
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