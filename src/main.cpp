#include <Sensor.h>
#include <Wire.h>

int serial_instruction = 0; // for incoming serial data

void setup()
{
  Serial.begin(115200);  

  // Activate i2c
  Wire1.begin();
  
  // Initialize devices
  Sensor::initialize();
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
        Accelerometer *accelerometer = Sensor::getAccelerometer();

        if (accelerometer->initialize()) {
          Serial.println("LSM9DS1: accelerometer and gyroscope are online");
          SensorData a = accelerometer->getAcceleration();
          
          Serial.print(a.x);
          Serial.print(", ");
          Serial.print(a.y);
          Serial.print(", ");
          Serial.println(a.z);
        }
        else
        {
          Serial.println("Accelerometer and gyroscope access failed!");
        }
        /*
        if (!IMU.statusMag()) {
          Serial.println("Magnetometer access failed!");
        }
        else
        {
          Serial.println("LSM9DS1: magnetometer is online");
        }
        */       
      break;
    }
  }
}