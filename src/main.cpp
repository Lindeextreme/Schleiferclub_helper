#include <Arduino.h>
#include <BlynkSimpleBLE_Nano_33.h>

#include "LSM9DS1.h"

const char auth[] = "ELVg7FH5pg-1x_Kne8vd-1gh9hrKjVsp";

int serial_instruction = 0; // for incoming serial data
LSM9DS1Class IMU(Wire1);

void setup()
{
  Serial.begin(115200);
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Blynk.begin(auth);
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

  BLEDevice central = BLE.central();

  if(central) {
    Blynk.run();
  }

}

BLYNK_READ(V0)
{
  Blynk.virtualWrite(V0, millis() / 100); //sending to Blynk
}

BLYNK_READ(V1)
{
  Blynk.virtualWrite(V1, millis() / 200); //sending to Blynk
}

BLYNK_READ(V2)
{
  Blynk.virtualWrite(V2, millis() / 500); //sending to Blynk
}

BLYNK_READ(V5)
{
  Blynk.virtualWrite(V5, millis() / 1000); //sending to Blynk
}