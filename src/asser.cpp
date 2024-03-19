#include "asser.hpp"
#include <iostream>

using namespace std;

// Takes in input an array of ints to convert to an array of uint8_t LSB first then MSB (Little Endian)
static void generateBytes(int *values, size_t length, uint8_t *result) {
    for (size_t i = 0; i < (length/2); i++) {
        uint8_t resultMSB, resultLSB;
        resultMSB = (uint8_t)(values[i] & 0xFF);
        resultLSB = (uint8_t)((values[i] >> 8) & 0xFF);
        result[2 * i] = resultLSB;
        result[2 * i + 1] = resultMSB;
    }
}

Asser::Asser(int slave_address) {
    int adapter_nr = 1; /* probably dynamically determined */
    char filename[20];

    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
    i2cFile = open(filename, O_RDWR);
    if (i2cFile < 0) {
        /* ERROR HANDLING; you can check errno to see what went wrong */
        cout << "Couldn't open I2C file\n";
        exit(1);
    }

    if (ioctl(i2cFile, I2C_SLAVE, slave_address) < 0) {
        cout << "ioctl failed\n";
        exit(1);
    }
}

int Asser::turnOnLed(int ledN) {
    uint8_t message = (ledN == 1) ? 10 : 12;
    if (i2c_smbus_write_byte(i2cFile, (char)message)) {
        cout << "Error: couldn't turn on LED\n";
        return 1;
    }
    return 0;
}

int Asser::turnOffLed(int ledN) {
    uint8_t message = (ledN == 1) ? 11 : 13;
    if (i2c_smbus_write_byte(i2cFile, (char)message)) {
        cout << "Error: couldn't turn off LED\n";
        return 1;
    }
    return 0;
}

int Asser::stop() {
    return i2c_smbus_write_byte(i2cFile, (char)30);
}

int Asser::linearSetpoint(int x, int y) {
    int length = 4;  // Nb of bytes to send
    uint8_t message[4];
    int values[] = {x, y};

    generateBytes(values, length, message);
    for(int i =0; i<4;i++)printf("%d\n",message[i]);
    i2c_smbus_write_i2c_block_data(i2cFile, (uint8_t)31, length, message);
    return 0;
}

int Asser::angularSetpoint(int angle, int rotation) {
    int length = 4;  // Nb of bytes to send
    uint8_t message[4];
    int values[] = {angle, rotation};
    generateBytes(values, length, message);
    for(int i =0; i<4;i++)printf("%d\n",message[i]);
    i2c_smbus_write_i2c_block_data(i2cFile, (uint8_t)32, length, message);
    return 0;
}

int Asser::setLookForward(int x, int y, int rotation) {
    int length = 6;  // Nb of bytes to send
    uint8_t message[6];
    int values[] = {x, y, rotation};

    generateBytes(values, length, message);
    i2c_smbus_write_i2c_block_data(i2cFile, (uint8_t)33, length, message);
    return 0;
}

int Asser::setLookBackward(int x, int y, int rotation) {
    int length = 6;  // Nb of bytes to send
    uint8_t message[6];
    int values[] = {x, y, rotation};

    generateBytes(values, length, message);
    i2c_smbus_write_i2c_block_data(i2cFile, (uint8_t)34, length, message);
    return 0;
}

int Asser::setCoords(int x, int y, int z){
    int length = 6; // Nb of bytes to send
    uint8_t message[6];
    int values[] = {x, y, z};

    generateBytes(values, length, message);
    i2c_smbus_write_i2c_block_data(i2cFile, (uint8_t) 21, length, message);
    return 0;
}


int Asser::getPostion(uint8_t* buffer, int length) {
    // La longueur indique le nombre de bytes à recevoir
    // buffer est un pointeur vers un tableau de uint8_t où stocker les données reçues
    //printf("test");

    i2c_smbus_write_byte(i2cFile, 20);
    int bytesRead = read(i2cFile, buffer, 6);

    //int bytesRead = i2c_smbus_read_i2c_block_data(i2cFile, (uint8_t)20, 6, buffer);
    
    // Vérification si la lecture a réussi
    if (bytesRead != length) {
        // La lecture n'a pas réussi correctement
        // Gérer l'erreur ici
        return -1; // Ou tout autre code d'erreur que vous préférez
    }
    
    return 0; // La lecture a réussi
}