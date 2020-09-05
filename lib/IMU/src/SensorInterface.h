#pragma once

#include <stdint.h>

class SensorInterface {
    public:
        // Constructor
        SensorInterface(uint8_t address);

    protected:
        bool readByte(uint8_t regAddr, uint8_t *data);
        bool readBytes(uint8_t regAddr, uint8_t length, uint8_t *data);

        bool writeByte(uint8_t regAddr, uint8_t data);
        bool writeBytes(uint8_t regAddr, uint8_t length, uint8_t *data);

        void usleep(unsigned int us);

        uint8_t address;
};