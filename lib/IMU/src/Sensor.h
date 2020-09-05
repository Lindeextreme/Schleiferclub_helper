#pragma once

#include "Accelerometer.h"

class Sensor {
public:
    // Initialize attached sensors
    // Only call this after enabling and waking up the I2C bus
    static void initialize() {
        Accelerometer::getInstance();
    }

    static Accelerometer *getAccelerometer() {
        return &Accelerometer::getInstance();
    }    
};