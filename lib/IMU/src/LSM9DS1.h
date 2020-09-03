#include <Arduino.h>
#include <Wire.h>

enum class Acc_scale_settings {  // set of allowable accel full scale settings
  AFS_2G = 0,
  AFS_16G,
  AFS_4G,
  AFS_8G
};

enum class Acc_sample_rates {  // set of allowable gyro sample rates
  AODR_PowerDown = 0,
  AODR_10Hz,
  AODR_50Hz,
  AODR_119Hz,
  AODR_238Hz,
  AODR_476Hz,
  AODR_952Hz
};

enum class Acc_data_bandwiths {  // set of allowable accewl bandwidths
  ABW_408Hz = 0,
  ABW_211Hz,
  ABW_105Hz,
  ABW_50Hz
};

enum class Gyro_scale_settings {  // set of allowable gyro full scale settings
  GFS_245DPS = 0,
  GFS_500DPS,
  GFS_NoOp,
  GFS_2000DPS
};

enum class Gyro_sample_rates {  // set of allowable gyro sample rates
  GODR_PowerDown = 0,
  GODR_14_9Hz,
  GODR_59_5Hz,
  GODR_119Hz,
  GODR_238Hz,
  GODR_476Hz,
  GODR_952Hz
};

enum class Gyro_data_bandwiths {   // set of allowable gyro data bandwidths
  GBW_low = 0,  // 14 Hz at Godr = 238 Hz,  33 Hz at Godr = 952 Hz
  GBW_med,      // 29 Hz at Godr = 238 Hz,  40 Hz at Godr = 952 Hz
  GBW_high,     // 63 Hz at Godr = 238 Hz,  58 Hz at Godr = 952 Hz
  GBW_highest   // 78 Hz at Godr = 238 Hz, 100 Hz at Godr = 952 Hz
};

enum class Mag_scale_settings {  // set of allowable mag full scale settings
  MFS_4G = 0,
  MFS_8G,
  MFS_12G,
  MFS_16G
};

enum class Mag_operation_modes {
  MMode_LowPower = 0, 
  MMode_MedPerformance,
  MMode_HighPerformance,
  MMode_UltraHighPerformance
};

enum class Mag_sample_rates {  // set of allowable mag sample rates
  MODR_0_625Hz = 0,
  MODR_1_25Hz,
  MODR_2_5Hz,
  MODR_5Hz,
  MODR_10Hz,
  MODR_20Hz,
  MODR_80Hz
};

class LSM9DS1Class {
  public:
    LSM9DS1Class(TwoWire& wire);
    virtual ~LSM9DS1Class();
    int begin();
    
    int statusAccGyro();
    void selfTestAcc();
    void selfTestGyro();

    int statusMag();

    
  private:
    int readRegister(uint8_t slaveAddress, uint8_t address);
    void readBytes(uint8_t slaveAddress, uint8_t address, uint8_t count, uint8_t * dest);
    int writeRegister(uint8_t slaveAddress, uint8_t address, uint8_t value);
    
    void calibrateAcc(float * ret_val);
    void calibrateGyro(float * ret_val);
  private:
    TwoWire* _wire;

    const Acc_scale_settings _acc_scale = Acc_scale_settings::AFS_2G;
    const Acc_sample_rates _acc_sample_rate = Acc_sample_rates::AODR_238Hz;
    const Acc_data_bandwiths _acc_data_bandwith = Acc_data_bandwiths::ABW_50Hz;
    float _acc_resolution;

    const Gyro_scale_settings _gyro_scale = Gyro_scale_settings::GFS_245DPS;
    const Gyro_sample_rates _gyro_sample_rate = Gyro_sample_rates::GODR_238Hz;
    const Gyro_data_bandwiths _gyro_data_bandwith = Gyro_data_bandwiths::GBW_med;
    float _gyro_resolution;

    const Mag_scale_settings _mag_scale = Mag_scale_settings::MFS_4G;
    const Mag_sample_rates _mag_sample_rate = Mag_sample_rates::MODR_10Hz;
    const Mag_operation_modes _mag_operation_mode =  Mag_operation_modes::MMode_HighPerformance;
    float _mag_resolution;
};

extern LSM9DS1Class IMU;