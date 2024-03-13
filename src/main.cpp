#include <iostream>
#include "fonction.h"
#include "asser.hpp"
using namespace std;

// I2C setup temp

int main(){
  Asser *robot = new Asser();

  robot->turnOnLed(1);
  robot->turnOnLed(2);
  robot->angularSetpoint(90, 0);
  
}