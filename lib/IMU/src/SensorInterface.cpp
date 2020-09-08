#include "Arduino.h"
#include <Wire.h>

#include "SensorInterface.h"

SensorInterface::SensorInterface(uint8_t address) : address(address) {}

bool SensorInterface::readByte(uint8_t regAddr, uint8_t *data) {
    return readBytes(regAddr, 1, data);
}

bool SensorInterface::readBytes(uint8_t regAddr, uint8_t length, uint8_t *data) {
    uint8_t count = 0;

    // Select the slave address and register to read from
    Wire1.beginTransmission(address);
    Wire1.write(regAddr);
    Wire1.endTransmission();

    // Request length bytes of data
    Wire1.requestFrom(address, length);
    while(Wire1.available()) {
        data[count] = Wire1.read();
        count++;
    }

    return (count == length);
}

bool SensorInterface::writeByte(uint8_t regAddr, uint8_t data) {
    return writeBytes(regAddr, 1, &data);
}

bool SensorInterface::writeBytes(uint8_t regAddr, uint8_t length, uint8_t *data) {
    // Select the slave address and register to write to
    Wire1.beginTransmission(address);
    Wire1.write(regAddr);

    // Write the data
    Wire1.write(data, length);

    // Finish transmission and return the status
    return (Wire1.endTransmission() == 0);
}

void SensorInterface::usleep(unsigned int us) {
    //delayMicroseconds(us);
    delay(us);
}
