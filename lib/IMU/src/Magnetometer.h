#pragma once

#include "SensorInterface.h"
#include "SensorData.h"

#define LSM9DS1M_ADDRESS      0x1E

enum class Scale_settings {  // set of allowable mag full scale settings
  MFS_4G = 0,
  MFS_8G,
  MFS_12G,
  MFS_16G
};

enum class Sample_rates {  // set of allowable mag sample rates
  MODR_0_625Hz = 0,
  MODR_1_25Hz,
  MODR_2_5Hz,
  MODR_5Hz,
  MODR_10Hz,
  MODR_20Hz,
  MODR_80Hz
};

enum class Data_bandwiths {   // set of allowable mag data bandwidths
  MMode_LowPower = 0, 
  MMode_MedPerformance,
  MMode_HighPerformance,
  MMode_UltraHighPerformance
};

class Gyroscope : public SensorInterface {
    public:
        static Gyroscope& getInstance() {
            static Gyroscope instance;
            return instance;
        }
                // Initialization
        Gyroscope(uint8_t address = LSM9DS1M_ADDRESS) : SensorInterface(address) {
            switch(scale)
            {
                case Scale_settings::MFS_4G:
                resolution = 4.0/32768.0;
                break;
                case Scale_settings::MFS_8G:
                resolution = 8.0/32768.0;
                break;
                case Scale_settings::MFS_12G:
                resolution = 12.0/32768.0;
                break;
                case Scale_settings::MFS_16G:
                resolution = 16.0/32768.0;
                break;                
            }
        }
    
    private:   

    private:
        bool is_initialized;
        const Scale_settings scale = Scale_settings::MFS_4G;
        const Sample_rates sample_rate = Sample_rates::MODR_10Hz;
        const Data_bandwiths data_bandwith = Data_bandwiths::MMode_HighPerformance;

        float resolution;
};
