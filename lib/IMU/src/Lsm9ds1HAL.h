#pragma once

#include "I2CWrapper.h"

#define LSM9DS1XG_ADDRESS      0x6B
#define LSM9DS1XG_CTRL_REG1_G  0x10
#define LSM9DS1XG_CTRL_REG4    0x1E
#define LSM9DS1XG_CTRL_REG5_XL 0x1F
#define LSM9DS1XG_CTRL_REG6_XL 0x20
#define LSM9DS1XG_CTRL_REG8    0x22
#define LSM9DS1XG_CTRL_REG9    0x23
#define LSM9DS1XG_CTRL_REG10   0x24
#define LSM9DS1XG_FIFO_CTRL    0x2E
#define LSM9DS1XG_FIFO_SRC     0x2F
#define LSM9DS1XG_OUT_X_L_XL   0x28
#define LSM9DS1XG_OUT_X_L_G    0x18
#define LSM9DS1XG_WHO_AM_I     0x0F

enum class SensorType {  // set of allowable gyro sample rates
  Accelerometer,
  Gyroscope,
};

struct SensorData16Bit {
    union {
        int16_t v[3];
        struct {
            int16_t x;
            int16_t y;
            int16_t z;
        };
    };

    SensorData16Bit() : x(0), y(0), z(0) { }
};

struct SensorData32Bit {
    union {
        int32_t v[3];
        struct {
            int32_t x;
            int32_t y;
            int32_t z;
        };
    };

    SensorData32Bit() : x(0), y(0), z(0) { }
};

struct SensorDataFloat {
    union {
        float v[3];
        struct {
            float x;
            float y;
            float z;
        };
    };

    SensorDataFloat() : x(0), y(0), z(0) { }
};

class Lsm9ds1HAL : public I2CWrapper {
    public:

        static Lsm9ds1HAL& getInstance() {
            static Lsm9ds1HAL instance;
            return instance;
        }    
        
        // Initialization
        Lsm9ds1HAL(uint8_t address = LSM9DS1XG_ADDRESS) : I2CWrapper(address) {
            is_sw_reset_performed = false;
            is_fifo_enabled = false;
        }
    
        bool initSensor(SensorType s_type, uint8_t config);
        
        void selfTestEnabled(void);
        void selfTestDisabled(void);

        void fifoModeEnabled(void);
        void fifoModeDisabled(void);

        void getRawData(SensorType s_type, SensorData16Bit *data);
        
        void getFifoMeanValue(SensorType s_type, SensorData32Bit *mean_data);

    private:
        void resetDevice(void);

    private:
        bool is_sw_reset_performed;
        bool is_fifo_enabled;
        uint8_t readBuffer[6];
};
