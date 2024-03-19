#include <iostream>
#include "fonction.h"
#include "asser.hpp"
using namespace std;

// I2C setup temp

int main(){
  Asser *robot = new Asser(I2C_ASSER_ADDR);

  robot->turnOnLed(1);
  robot->turnOnLed(2);
  //robot->angularSetpoint(45, 0);
  robot->linearSetpoint(600,0);

  uint8_t buffer[20];
  robot->getPostion(buffer,20);

  for(int i =0; i < 0; i++){
    printf("data out : %d\n",buffer[i]);
  }
  
}