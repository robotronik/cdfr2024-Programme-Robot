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
    // for(int i =0; i<4;i++)printf("%d\n",message[i]);
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


int Asser::getCoords(int &x, int &y, int &theta) {
    // La longueur indique le nombre de bytes à recevoir
    // buffer est un pointeur vers un tableau de uint8_t où stocker les données reçues
    //printf("test");
    uint8_t buffer[6];

    i2c_smbus_write_byte(i2cFile, 20);
    int bytesRead = read(i2cFile, buffer, 6);
    //int bytesRead = i2c_smbus_read_i2c_block_data(i2cFile, 20, 6, buffer);

    uint8_t resultMSB, resultLSB;
    resultLSB = buffer[2 * 0];
    resultMSB = buffer[2 * 0 + 1];
    x = resultMSB<<8 | resultLSB;
    printf("x : %d\n",x );

    resultLSB = buffer[2 * 1];
    resultMSB = buffer[2 * 1 + 1];
    y = resultMSB<<8 | resultLSB;
    printf("y : %d\n",y );

    resultLSB = buffer[2 * 2];
    resultMSB = buffer[2 * 2 + 1];
    theta = resultMSB<<8 | resultLSB;
    printf("teta : %d\n",theta );
    
    // Vérification si la lecture a réussi
    if (bytesRead != 6) {
        // La lecture n'a pas réussi correctement
        // Gérer l'erreur ici
        return -1; // Ou tout autre code d'erreur que vous préférez
    }
    
    return 0; // La lecture a réussi
}

int Asser::servo1Position(int position) {
    int length = 2;  // Nb of bytes to send
    uint8_t message[2];
    int values[] = {position};

    if (ioctl(i2cFile, I2C_SLAVE, 100) < 0) {
        cout << "ioctl failed\n";
        exit(1);
    }

    generateBytes(values, length, message);
    for(int i =0; i<2;i++)printf("%d\n",message[i]);
    i2c_smbus_write_i2c_block_data(i2cFile, (uint8_t)1, length, message);
    return 0;
}

bool Asser::getRobotFinished(status_finished status){
    int command = 40 + status;
    uint8_t buffer[2];

    i2c_smbus_write_byte(i2cFile, command);
    read(i2cFile, buffer, 2);

    return (buffer[1] + buffer[2]) != 0;
}

int Asser::getBrakingDistance(int &distance){
    uint8_t buffer[2];

    i2c_smbus_write_byte(i2cFile, 45);
    int bytesRead = read(i2cFile, buffer, 2);
    //int bytesRead = i2c_smbus_read_i2c_block_data(i2cFile, 20, 6, buffer);

    uint8_t resultMSB, resultLSB;
    resultLSB = buffer[2 * 0];
    resultMSB = buffer[2 * 0 + 1];
    distance = resultMSB<<8 | resultLSB;
    printf("Braking distance : %d\n", distance);
    
    // Vérification si la lecture a réussi
    if (bytesRead != 2) {
        // La lecture n'a pas réussi correctement
        // Gérer l'erreur ici
        return -1; // Ou tout autre code d'erreur que vous préférez
    }
    
    return 0; // La lecture a réussi
}

int Asser::getError(asser_error_type error_type, int &error){
    uint8_t buffer[2];
    int command = 40 + error_type;

    i2c_smbus_write_byte(i2cFile, command);
    int bytesRead = read(i2cFile, buffer, 2);
    //int bytesRead = i2c_smbus_read_i2c_block_data(i2cFile, 20, 6, buffer);

    uint8_t resultMSB, resultLSB;
    resultLSB = buffer[2 * 0];
    resultMSB = buffer[2 * 0 + 1];
    error = resultMSB<<8 | resultLSB;
    printf("Error : %d\n", error);
    
    // Vérification si la lecture a réussi
    if (bytesRead != 2) {
        // La lecture n'a pas réussi correctement
        // Gérer l'erreur ici
        return -1; // Ou tout autre code d'erreur que vous préférez
    }
    
    return 0; // La lecture a réussi
}