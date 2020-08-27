#include <Arduino.h>
#include <Wire.h>

class LSM9DS1Class {
  public:
    LSM9DS1Class(TwoWire& wire);
    virtual ~LSM9DS1Class();
    int begin();
    int statusAccGyro();
    int statusMag();
  
  private:
    int readRegister(uint8_t slaveAddress, uint8_t address);
    int writeRegister(uint8_t slaveAddress, uint8_t address, uint8_t value);
  private:
    TwoWire* _wire;
};

extern LSM9DS1Class IMU;