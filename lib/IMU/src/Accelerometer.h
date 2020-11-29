#pragma once

#include "Lsm9ds1HAL.h"

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

class Accelerometer{
    public:
        static Accelerometer& getInstance() {
            static Accelerometer instance;
            return instance;
        }
        // Initialization
        Accelerometer() {
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
        SensorDataFloat getAcceleration(void);
        SensorDataFloat selfTest(void);
    
    private:
        void getBias(SensorDataFloat * data);     

    private:
        Lsm9ds1HAL *hal;
        bool is_initialized;
        const Scale_settings scale = Scale_settings::AFS_2G;
        const Sample_rates sample_rate = Sample_rates::AODR_238Hz;
        const Data_bandwiths data_bandwith = Data_bandwiths::ABW_50Hz; 
        
        float resolution;
};
