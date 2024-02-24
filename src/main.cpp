#include <stdio.h>
#include <iostream>

#include "fonction.h"
// #include "asser.h"
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <smbus.h>
using namespace std;

// I2C setup temp

int main(){
  int file;
  int adapter_nr = 1; /* probably dynamically determined */
  char filename[20];

  snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
  file = open(filename, O_RDWR);
  if (file < 0) {
    /* ERROR HANDLING; you can check errno to see what went wrong */
    cout << "Couldn't open I2C file\n";
    exit(1);
  }

  int addr = 42;
  if (ioctl(file, I2C_SLAVE, addr) < 0) {
    cout << "ioctl failed\n";
    exit(1);
    }

    if (i2c_smbus_write_quick(file, 10)){
        cout << "Error: couldn't send byte\n";
    }
    // __u8 reg = 0x10;
//     __s32 res;
//     char buf[10];res = i2c_smbus_read_word_data(file, reg);
    // if (res < 0) {
    // /* ERROR HANDLING: i2c transaction failed */
    // cout << "I2C transaction failed\n";
    // } else {
    // /* res contains the read word */
    // }buf[0] = reg;
    // buf[1] = 0x43;
    // buf[2] = 0x65;
    // if (write(file, buf, 3) != 3) {
    // /* ERROR HANDLING: i2c transaction failed */
    // cout << "I2C transaction failed\n";
    // }
}