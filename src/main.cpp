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
  
}