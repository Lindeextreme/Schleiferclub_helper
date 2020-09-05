#pragma once

#include "SensorInterface.h"
#include "SensorData.h"

#define LSM9DS1XG_ADDRESS      0x6B
#define LSM9DS1XG_WHO_AM_I     0x0F
#define LSM9DS1XG_CTRL_REG8    0x22
#define LSM9DS1XG_CTRL_REG5_XL 0x1F
#define LSM9DS1XG_CTRL_REG6_XL 0x20

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

        bool initialize(void) 
        {
            uint8_t data;
            if(is_initialized == false)
            {
                /* Perform acc/gyr SW_RESET */
                writeByte(LSM9DS1XG_CTRL_REG8, 0x05);
                usleep(50);

                // Read WHO_AM_I register of LSM9DS1 accel/gyr
                readByte(LSM9DS1XG_WHO_AM_I, &data);

                if(data == 104)
                {
                    /* 
                    * Enable Xen_G -> Gyroscope’s pitch axis (X) output enable
                    * Enable Yen_G -> Gyroscope’s pitch axis (>) output enable
                    * Enable Zen_G -> Gyroscope’s pitch axis (Z) output enable
                    */
                    writeByte(LSM9DS1XG_CTRL_REG5_XL, 0x38);

                    /* Configure the accelerometer */
                    data = 0;
                    /* ODR_XL : Output data rate and power mode selection */
                    data |= static_cast<uint8_t>(sample_rate) << 5;
                    /* FS_XL : Accelerometer full-scale selection*/
                    data |= static_cast<uint8_t>(scale) << 3;
                    /* BW_XL : Anti-aliasing filter bandwidth selection */
                    data |= static_cast<uint8_t>(data_bandwith);
                    /* BW_SCAL_ODR : bandwidth selected according to BW_XL */
                    data |= 0x04;

                    writeByte(LSM9DS1XG_CTRL_REG6_XL, data);
                    usleep(200);
                    /*
                    * Enable BDU -> Block data update -> output registers not updated until MSB and LSB read
                    * Enable IF_ADD_INC -> Register address automatically incremented during a multiple byte access with a serial interface
                    */ 
                    writeByte(LSM9DS1XG_CTRL_REG8, 0x44);

                    is_initialized = true;
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                return 1;
            }
            
        }
        SensorData getAcceleration(void)
        {
            SensorData acc;

            if(is_initialized == true)
            {
                // Read raw measurement data
                int16_t rawData[3];
                getRawData(rawData);

                // Apply sensitivity adjustments, scale to get uT
                acc.x = rawData[0] * resolution;
                acc.y = rawData[1] * resolution;
                acc.z = rawData[2] * resolution;
            }
            else
            {
                acc.x = 0;
                acc.y = 0;
                acc.z = 0;
            }

            return acc;
        }

    private:
        bool is_initialized;
        const Scale_settings scale = Scale_settings::AFS_2G;
        const Sample_rates sample_rate = Sample_rates::AODR_238Hz;
        const Data_bandwiths data_bandwith = Data_bandwiths::ABW_50Hz;    
        
        float resolution;

        void getRawData(int16_t *data)
        {
            uint8_t buffer[6];

            readBytes(LSM9DS1XG_OUT_X_L_XL, 7, &buffer[0]);
            data[0] = (((int16_t)buffer[1]) << 8) | buffer[0];
            data[1] = (((int16_t)buffer[3]) << 8) | buffer[2];
            data[2] = (((int16_t)buffer[5]) << 8) | buffer[4];
        }
};
