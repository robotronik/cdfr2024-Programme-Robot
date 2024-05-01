#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <filesystem>

#include "fonction.h"
#include "lidarAnalize.h"
#include "lidar.h"
#include "asser.hpp"
#include "arduino.hpp"
#include "utils.h"
#include "arduinoSubFonction.h"
#include "logger.hpp"
#include "robot.h"

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

int main(int argc, char *argv[]) {
    LOG_INIT();


    if(!lidarSetup("/dev/ttyUSB0",256000)){
        return -1;
    }

    signal(SIGINT, ctrlc);
    signal(SIGTERM, ctrlc);


    LOG_DEBUG("test");
    robotCDFR mainRobot;
    LOG_DEBUG("test");
    Asser *robotI2C = new Asser(I2C_ASSER_ADDR);
    LOG_SETROBOT(robotI2C);
    lidarAnalize_t lidarData[SIZEDATALIDAR];    
    Arduino *arduino = new Arduino(100);
    main_State_t currentState = INIT;
    main_State_t nextState = INIT;
    unsigned long startTime;
    bool initStat;
    

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

    LOG_DEBUG("test");

    while (1) {

        LOG_SCOPE("Main");
        
       
        int count = SIZEDATALIDAR;
        if(getlidarData(lidarData,count)){
            int x, y, teta;
            int distance;
            robotI2C->getCoords(x,y,teta);
            position_t position = {x,y,teta,0};
            convertAngularToAxial(lidarData,count,position);
            //pixelArtPrint(lidarData,count,50,50,100,position);
            //printAngular(lidarData,count);
            robotI2C->getBrakingDistance(distance);
            mainRobot.robotStatus.collide = collide(lidarData,count,distance);
            //LOG_DEBUG("collide : ", robot->collide);
            //printf("distance : %d \t collide : %d\n",distance,robot->collide);
        }

       
       


        switch (currentState) {
            //****************************************************************
            case INIT:
                if(initStat) LOG_STATE("INIT");
                nextState = INITIALIZE;
                break;
            //****************************************************************
            case INITIALIZE:{
                if(initStat) LOG_STATE("INITIALIZE");
                if(initStat){
                    int bStateCapteur2;
                    arduino->readCapteur(2,bStateCapteur2);
                    mainRobot.robotStatus.colorTeam = bStateCapteur2 ? BLUE : YELLOW;
                    if(mainRobot.robotStatus.colorTeam  == YELLOW){
                        robotI2C->setCoords(800,1250,-90);
                        //robotI2C->setCoords(-1000,0,0);
                        printf("teams : YELLOW\n");
                    }
                    else{
                        robotI2C->setCoords(830,-1440,-90);
                        printf("teams : BLUE\n");
                    }
                    robotI2C->enableMotor(true);
                    arduino->enableStepper(1);
                    arduino->servoPosition(1,180);
                    arduino->servoPosition(2,CLAMPSLEEP);
                    arduino->moveStepper(ELEVATORUP,1);
                }
                //robotI2C->setCoords(0,1500,-90);   
                //robotI2C->linearSetpoint(0,1400);
                nextState = SETHOME;
                break;
            }
            //****************************************************************
            case SETHOME:{
                if(initStat) LOG_STATE("SETHOME");
                if(initPositon(robotI2C,800,1250,-90)){
                    nextState = WAITSTART;
                }
                break;
            }            
            case WAITSTART:{
                if(initStat) LOG_STATE("WAITSTART");
                int bStateCapteur1;
                arduino->readCapteur(1,bStateCapteur1);
                if(mainRobot.robotStatus.collide <= 500){
                    LOG_ERROR("Wait start collide : ",mainRobot.robotStatus.collide);
                }
                else if(bStateCapteur1 == 0 && mainRobot.robotStatus.collide > 500){
                    nextState = START;
                }
                break;
            }
            //****************************************************************      
            case START:{
                if(initStat) LOG_STATE("START");
                startTime = millis();
                //LAUNCH PYTHON
                std::string color = mainRobot.robotStatus.colorTeam == YELLOW ? "YELLOW" : "BLUE";
                std::filesystem::path exe_path = std::filesystem::canonical(std::filesystem::path(argv[0])).parent_path();
                std::filesystem::path python_script_path = exe_path / "../startPAMI.py";
                std::string command = "python3 " + python_script_path.string() + " " +  color;
                std::system(command.c_str());
                //
                nextState = RUN;
                break;
            }
            //****************************************************************
            case RUN:{
                if(initStat) LOG_STATE("RUN");
                //bool finish = takePlant(mainRobot,robotI2C, arduino,0,-1000,0,3);
                bool finish =  FSMMatch(mainRobot,robotI2C, arduino);
                if(startTime+80000 < millis() || finish){
                    nextState = FIN;
                }
                break;
            }
            case RETURNHOME:{
                if(initStat) LOG_STATE("RETURNHOME");
                bool finish =  returnToHome(robotI2C);
                if(startTime+90000 < millis() || finish){
                    nextState = FIN;
                }
                break;
            }
            //****************************************************************
            case FIN:
                if(initStat) LOG_STATE("FIN");
                //arduino->servoPosition(2,180);
                nextState = STOP;
                break;
            //****************************************************************
            case STOP:
                if(initStat) LOG_STATE("STOP");
                nextState = STOP;
                break;
            //****************************************************************
            default:
                LOG_STATE("default");
                nextState = STOP;
                break;
        }

        initStat = false;
        if(currentState != nextState){
            initStat = true;
        }
        currentState = nextState;


        //ctrl_c_pressed |= turnSolarPannel(robotI2C,arduino);

        if (ctrl_c_pressed){ 
            break;
        }
    }

    arduino->servoPosition(1,180);
    arduino->servoPosition(2,CLAMPSTOP);
    arduino->moveStepper(0,1);
    robotI2C->enableMotor(false);
    robotI2C->stop();
    lidarStop();
    arduino->disableStepper(1);

    return 0;
}