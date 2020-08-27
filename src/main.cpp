#include <Arduino.h>

#include "LSM9DS1.h"

int serial_instruction = 0; // for incoming serial data
LSM9DS1Class IMU(Wire1);

void setup()
{
  Serial.begin(115200);
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void loop()
{  
  if (Serial.available() > 0) {
    // read the incoming byte:
    serial_instruction = Serial.read();

    switch(serial_instruction)
    {
      case 63: /* -?, -- command details */
        Serial.println("Device: Arduino Nano 33 BLE");
        Serial.println("Firmware: Schleiferclub_helper V0.1");
        Serial.println();
        Serial.println("Following commands are available");
        Serial.println("-s, -- device status");
        Serial.println("-c, -- magnetometer calibration");
        Serial.println();
      break;
      case 99: /* -c, -- magnetometer calibration */
        Serial.println("Magnetometer calibration:");
      break;
      case 115: /* -s, -- device status */
        Serial.println("Device status:");
          if (!IMU.statusAccGyro()) {
            Serial.println("Accelerometer and gyroscope access failed!");
          }
          else
          {
            Serial.println("LSM9DS1: accelerometer and gyroscope are online");
          }

          if (!IMU.statusMag()) {
            Serial.println("Magnetometer access failed!");
          }
          else
          {
            Serial.println("LSM9DS1: magnetometer is online");
          }          
      break;
      default:
        Serial.println("Please type ? for more details");
        Serial.println();
      break;
    }
  }
}