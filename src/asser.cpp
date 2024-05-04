#include "asser.hpp"
#include <iostream>
#include "logger.hpp"

using namespace std;

Asser::Asser(int slave_address) :I2CDevice(slave_address) {
    
}

int Asser::turnOnLed(int ledN) {
    LOG_DEBUG("linear n° ",ledN," off");
    uint8_t message = (ledN == 1) ? 10 : 12;
    if (i2c_smbus_write_byte(i2cFile, (char)message)) {
        LOG_ERROR("Error: couldn't turn on LED\n");
        return 1;
    }
    return 0;
}

int Asser::turnOffLed(int ledN) {
    LOG_DEBUG("linear n° ",ledN," on");
    uint8_t message = (ledN == 1) ? 11 : 13;
    if (i2c_smbus_write_byte(i2cFile, (char)message)) {
        LOG_ERROR("Error: couldn't turn off LED\n");
        return 1;
    }
    return 0;
}

int Asser::stop() {
    return i2c_smbus_write_byte(i2cFile, (char)30);
}

int Asser::linearSetpoint(int x, int y) {
    LOG_INFO("linear set point : ",x," ",y);
    int length = 4;  // Nb of bytes to send
    uint8_t message[4];
    int values[] = {x, y};
    generateBytes(values, length, message);
    if (i2c_smbus_write_i2c_block_data(i2cFile, (uint8_t)31, length, message)){
        LOG_ERROR("Error: couldn't set linear point\n");
        return 1;
    }
    return 0;
}

int Asser::angularSetpoint(int angle, int rotation) {
    LOG_INFO("set lookat forward : ",angle," ",rotation);
    int length = 4;  // Nb of bytes to send
    uint8_t message[4];
    int values[] = {angle, rotation};
    generateBytes(values, length, message);
    if(i2c_smbus_write_i2c_block_data(i2cFile, (uint8_t)32, length, message)){
        LOG_ERROR("Error: couldn't set angular point\n");
        return 1;
    }
    return 0;
}

int Asser::setLookForward(int x, int y, int rotation) {
    LOG_INFO("set lookat forward : ",x," ",y," ",rotation);
    int length = 6;  // Nb of bytes to send
    uint8_t message[6];
    int values[] = {x, y, rotation};

    generateBytes(values, length, message);
    if(i2c_smbus_write_i2c_block_data(i2cFile, (uint8_t)33, length, message)){
        LOG_ERROR("Error: couldn't set lookat forward\n");
        return 1;
    }
    return 0;
}

int Asser::setLookBackward(int x, int y, int rotation) {
    LOG_INFO("set lookat backward : ",x," ",y," ",rotation);
    int length = 6;  // Nb of bytes to send
    uint8_t message[6];
    int values[] = {x, y, rotation};

    generateBytes(values, length, message);
    if(i2c_smbus_write_i2c_block_data(i2cFile, (uint8_t)34, length, message)){
        LOG_ERROR("Error: couldn't set lookat backward\n");
        return 1;
    }
    return 0;
}

int Asser::setCoords(int x, int y, int z){
    LOG_INFO("Set Coordonates : ",x," ",y," ",z);
    int length = 6; // Nb of bytes to send
    uint8_t message[6];
    int values[] = {x, y, z};

    generateBytes(values, length, message);
    if(i2c_smbus_write_i2c_block_data(i2cFile, (uint8_t) 21, length, message)){
        LOG_ERROR("Error: couldn't set coord\n");
        return 1;
    }
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
    x = (int16_t) (resultMSB<<8 | resultLSB);
    // printf("x : %d\n",x );

    resultLSB = buffer[2 * 1];
    resultMSB = buffer[2 * 1 + 1];
    y = (int16_t) (resultMSB<<8 | resultLSB);
    // printf("y : %d\n",y );

    resultLSB = buffer[2 * 2];
    resultMSB = buffer[2 * 2 + 1];
    theta = (int16_t) (resultMSB<<8 | resultLSB);
    // printf("teta : %d\n",theta );

    if (bytesRead != 6) {
        // La lecture n'a pas réussi correctement
        // Gérer l'erreur ici
        LOG_ERROR("could not get coordonate");
        return -1; // Ou tout autre code d'erreur que vous préférez
    }
    
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

    i2c_smbus_write_byte(i2cFile, 45);
    int bytesRead = read(i2cFile, buffer, 2);
    //int bytesRead = i2c_smbus_read_i2c_block_data(i2cFile, 20, 6, buffer);

    uint8_t resultMSB, resultLSB;
    resultLSB = buffer[2 * 0];
    resultMSB = buffer[2 * 0 + 1];
    distance = (int16_t)(resultMSB<<8 | resultLSB);
    
    // Vérification si la lecture a réussi
    if (bytesRead != 2) {
        LOG_ERROR("get Braking Distance not reply");
        return -1; 
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
    // printf("Error : %d\n", error);
    
    // Vérification si la lecture a réussi
    if (bytesRead != 2) {
        // La lecture n'a pas réussi correctement
        // Gérer l'erreur ici
        return -1; // Ou tout autre code d'erreur que vous préférez
    }
    return error; // La lecture a réussi
}

int Asser::getError(asser_error_type error_type){
    int ivalRet;
    this->getError(error_type,ivalRet);
    return ivalRet;
}

int Asser::enableMotor(bool status) {
    printf("%s motor\n",status?"Enable":"disable");
    uint8_t message = (status == true) ? 51 : 50;
    if (i2c_smbus_write_byte(i2cFile, (char)message)) {
        printf("Error: couldn't stop motor\n");
        return 1;
    }
    return 0;
}

int Asser::setLinearMaxSpeed(int maxSpeed) {
    LOG_DEBUG("set linear max speed : ",maxSpeed);
    int length = 2;
    uint8_t message[2];
    int values[] = {maxSpeed};
    generateBytes(values, length, message);
    if(i2c_smbus_write_i2c_block_data(i2cFile, (uint8_t)60, length, message)){
        LOG_ERROR("Error: couldn't set angular point\n");
    }
    if(i2c_smbus_write_i2c_block_data(i2cFile, (uint8_t)61, length, message)){
        LOG_ERROR("Error: couldn't set angular point\n");
    }
    return 0;
}

int Asser::setAngularMaxSpeed(int maxSpeed) {
    LOG_DEBUG("set angular max speed : ",maxSpeed);
    int length = 2;
    uint8_t message[2];
    int values[] = {maxSpeed};
    generateBytes(values, length, message);
    if(i2c_smbus_write_i2c_block_data(i2cFile, (uint8_t)62, length, message)){
        LOG_ERROR("Error: couldn't set angular point\n");
    }
     if(i2c_smbus_write_i2c_block_data(i2cFile, (uint8_t)63, length, message)){
        LOG_ERROR("Error: couldn't set angular point\n");
    }
    return 0;
}