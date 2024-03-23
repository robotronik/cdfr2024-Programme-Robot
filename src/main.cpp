#include "asser.hpp"
#include "arduino.hpp"
#include "fonction.h"
#include <iostream>
#include <unistd.h>
using namespace std;

// I2C setup temp

int main() {
  Asser *robot = new Asser(I2C_ASSER_ADDR);
  Arduino *servo = new Arduino(100);

  robot->angularSetpoint(15, 2);
  sleep(1);
  servo->servo1Position(135);
  sleep(1);
  robot->linearSetpoint(0,0);
  sleep(1);
  servo->servo1Position(0);
  int x, y, theta, angularError, linearError;
  robot->getCoords(x, y, theta);
  robot->getError(ANGULAR_ERROR, angularError);
  robot->getError(LINEAR_ERROR, linearError);

  // for(int i =0; i < 6; i++){
  //   printf("data out : %d\n",buffer[i]);
  // }

  cout << "x: " << x << endl
       << "y: " << y << endl
       << "theta: " << theta << endl
       << "ang error: " << angularError << endl
       << "linear error: " <<  linearError << endl;

  // for (size_t i = 0; i < 10; i++)
  // {
  //   int tmp = i % 4;
  //   servo->servo1Position(tmp*45);
  //   cout << "Angle " << tmp*45 << endl;
  //   sleep(2);
  // }
  

  // servo->servo1Position(90);
}