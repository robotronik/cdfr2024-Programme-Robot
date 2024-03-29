#include "asser.hpp"
#include <iostream>

using namespace std;

Asser::Asser(int slave_address) :I2CDevice(slave_address) {
    
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
    // for(int i =0; i<4;i++)printf("%d\n",message[i]);
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
    int16_t wordBuffer[3];

    i2c_smbus_write_byte(i2cFile, 20);
    int bytesRead = read(i2cFile, buffer, 6);
    //int bytesRead = i2c_smbus_read_i2c_block_data(i2cFile, 20, 6, buffer);

    bytesToWords(buffer, wordBuffer, 6);
    x = wordBuffer[0];
    y = wordBuffer[1];
    theta = wordBuffer[3];
    
    // Vérification si la lecture a réussi
    if (bytesRead != 6) {
        // La lecture n'a pas réussi correctement
        // Gérer l'erreur ici
        return -1; // Ou tout autre code d'erreur que vous préférez
    }
    
    return 0; // La lecture a réussi
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
    int16_t wordBuffer;

    i2c_smbus_write_byte(i2cFile, 45);
    int bytesRead = read(i2cFile, buffer, 2);
    //int bytesRead = i2c_smbus_read_i2c_block_data(i2cFile, 20, 6, buffer);
    bytesToWords(buffer, &wordBuffer, 2);
    distance = wordBuffer;

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
    int16_t wordBuffer;
    int command = 43 + error_type;

    i2c_smbus_write_byte(i2cFile, command);
    int bytesRead = read(i2cFile, buffer, 2);
    //int bytesRead = i2c_smbus_read_i2c_block_data(i2cFile, 20, 6, buffer);
    bytesToWords(buffer, &wordBuffer, 6);

    error = wordBuffer;
    // printf("Error : %d\n", error);
    
    // Vérification si la lecture a réussi
    if (bytesRead != 2) {
        // La lecture n'a pas réussi correctement
        // Gérer l'erreur ici
        return -1; // Ou tout autre code d'erreur que vous préférez
    }
    
    return 0; // La lecture a réussi
}