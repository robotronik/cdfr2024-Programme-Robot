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

int Arduino::readCapteur(int capteurNumber, int &state){
    uint8_t buffer[2];
    int command = 100 + (capteurNumber -1);

    i2c_smbus_write_byte(i2cFile, command);
    int bytesRead = read(i2cFile, buffer, 2);
    //int bytesRead = i2c_smbus_read_i2c_block_data(i2cFile, 20, 6, buffer);

    uint8_t resultMSB, resultLSB;
    resultLSB = buffer[2 * 0];
    resultMSB = buffer[2 * 0 + 1];
    state = resultMSB<<8 | resultLSB;
    // printf("Error : %d\n", error);
    
    // Vérification si la lecture a réussi
    if (bytesRead != 2) {
        // La lecture n'a pas réussi correctement
        // Gérer l'erreur ici
        return -1; // Ou tout autre code d'erreur que vous préférez
    }
    
    return 0; // La lecture a réussi
}