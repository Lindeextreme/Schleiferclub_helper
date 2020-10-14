#pragma once

#include "SensorInterface.h"
#include "SensorData.h"

#define LSM9DS1XG_ADDRESS      0x6B
#define LSM9DS1XG_WHO_AM_I     0x0F
#define LSM9DS1XG_CTRL_REG8    0x22
#define LSM9DS1XG_CTRL_REG5_XL 0x1F
#define LSM9DS1XG_CTRL_REG6_XL 0x20
#define LSM9DS1XG_CTRL_REG9    0x23
#define LSM9DS1XG_CTRL_REG10   0x24
#define LSM9DS1XG_FIFO_CTRL    0x2E
#define LSM9DS1XG_FIFO_SRC     0x2F
#define LSM9DS1XG_OUT_X_L_XL   0x28

enum class Scale_settings {  // set of allowable accel full scale settings
  AFS_2G = 0,
  AFS_16G,
  AFS_4G,
  AFS_8G
};

enum class Sample_rates {  // set of allowable gyro sample rates
  AODR_PowerDown = 0,
  AODR_10Hz,
  AODR_50Hz,
  AODR_119Hz,
  AODR_238Hz,
  AODR_476Hz,
  AODR_952Hz
};

enum class Data_bandwiths {  // set of allowable accewl bandwidths
  ABW_408Hz = 0,
  ABW_211Hz,
  ABW_105Hz,
  ABW_50Hz
};

class Accelerometer : public SensorInterface {
    public:
        static Accelerometer& getInstance() {
            static Accelerometer instance;
            return instance;
        }
                // Initialization
        Accelerometer(uint8_t address = LSM9DS1XG_ADDRESS) : SensorInterface(address) {
            switch(scale)
            {
                case Scale_settings::AFS_2G:
                resolution = 2.0/32768.0;
                break;
                case Scale_settings::AFS_16G:
                resolution = 16.0/32768.0;
                break;
                case Scale_settings::AFS_4G:
                resolution = 4.0/32768.0;
                break;
                case Scale_settings::AFS_8G:
                resolution = 8.0/32768.0;
                break;
            }
        }

        bool initialize(void);
        SensorData getAcceleration(void);
        SensorData selfTest(void);
    
    private:
        void getRawData(void);

        void getBias(float * data);
        void enableFifoMode(void);
        void getFifoMeanValue(void);
        void disableFifoMode(void);        

    private:
        bool is_initialized;
        const Scale_settings scale = Scale_settings::AFS_2G;
        const Sample_rates sample_rate = Sample_rates::AODR_238Hz;
        const Data_bandwiths data_bandwith = Data_bandwiths::ABW_50Hz;    
        
        float resolution;
        uint8_t sensorReadBuffer[6];
        int16_t accRawData[3];
        int32_t accRawDataMeanVal[3];
        float biasSelfTest[3]; 
        float biasNoSelfTest[3];

        SensorData acc;
};
