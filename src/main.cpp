#include "asser.hpp"
#include "fonction.h"
#include <iostream>
using namespace std;

// I2C setup temp

int main() {
  Asser *robot = new Asser(I2C_ASSER_ADDR);

  robot->turnOnLed(1);
  robot->turnOnLed(2);
  // robot->angularSetpoint(15, 2);
  // robot->linearSetpoint(600,0);

  int x, y, theta;
  robot->getCoords(x, y, theta);

  // for(int i =0; i < 6; i++){
  //   printf("data out : %d\n",buffer[i]);
  // }

  cout << "x: " << x << endl
       << "y: " << y << endl
       << "theta: " << theta << endl;

  robot->servo1Position(0);
}