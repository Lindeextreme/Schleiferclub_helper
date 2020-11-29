#pragma once

#include "Lsm9ds1HAL.h"

enum class Scale_settings {  // set of allowable gyro full scale settings
  GFS_245DPS = 0,
  GFS_500DPS,
  GFS_NoOp,
  GFS_2000DPS
};

enum class Sample_rates {  // set of allowable gyro sample rates
  GODR_PowerDown = 0,
  GODR_14_9Hz,
  GODR_59_5Hz,
  GODR_119Hz,
  GODR_238Hz,
  GODR_476Hz,
  GODR_952Hz
};

enum class Data_bandwiths {   // set of allowable gyro data bandwidths
  GBW_low = 0,  // 14 Hz at Godr = 238 Hz,  33 Hz at Godr = 952 Hz
  GBW_med,      // 29 Hz at Godr = 238 Hz,  40 Hz at Godr = 952 Hz
  GBW_high,     // 63 Hz at Godr = 238 Hz,  58 Hz at Godr = 952 Hz
  GBW_highest   // 78 Hz at Godr = 238 Hz, 100 Hz at Godr = 952 Hz
};

class Gyroscope{
    public:
        static Gyroscope& getInstance() {
            static Gyroscope instance;
            return instance;
        }
        // Initialization
        Gyroscope() {

          switch(scale)
          {
              case Scale_settings::GFS_245DPS:
              resolution = 245.0/32768.0;
              break;
              case Scale_settings::GFS_500DPS:
              resolution = 500.0/32768.0;
              break;
              case Scale_settings::GFS_2000DPS:
              resolution = 2000.0/32768.0;
              break;
          }
        }

        bool initialize(void);
    
    private:   

    private:
        Lsm9ds1HAL *hal;
        bool is_initialized;
        const Scale_settings scale = Scale_settings::GFS_245DPS;
        const Sample_rates sample_rate = Sample_rates::GODR_238Hz;
        const Data_bandwiths data_bandwith = Data_bandwiths::GBW_med;

        float resolution;
};
