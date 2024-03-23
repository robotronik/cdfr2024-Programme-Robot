#include "arduino.hpp"

Arduino::Arduino(int slave_address) : I2CDevice (slave_address){}


int Arduino::servo1Position(int position) {
    int length = 2;  // Nb of bytes to send
    uint8_t message[2];
    int values[] = {position};

    // if (ioctl(i2cFile, I2C_SLAVE, 100) < 0) {
    //     cout << "ioctl failed\n";
    //     exit(1);
    // }

    generateBytes(values, length, message);
    for(int i =0; i<2;i++)printf("Dans la fonction: %d\n",message[i]);
    i2c_smbus_write_i2c_block_data(i2cFile, (uint8_t)1, length, message);
    return 0;
}