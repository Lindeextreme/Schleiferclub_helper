#include "Lsm9ds1HAL.h"
#include "Arduino.h"

void Lsm9ds1HAL::resetDevice(void)
{
    uint8_t data;

    if(is_sw_reset_performed == false)
    {
        /* Perform acc/gyr SW_RESET */
        writeByte(LSM9DS1XG_CTRL_REG8, 0x05);
        usleep(50);

        // Read WHO_AM_I register of LSM9DS1 accel/gyr
        readByte(LSM9DS1XG_WHO_AM_I, &data);

        if(data == 104)
        {
            /*
             * Enable BDU -> Block data update -> output registers not updated until MSB and LSB read
             * Enable IF_ADD_INC -> Register address automatically incremented during a multiple byte access with a serial interface
             */ 
            writeByte(LSM9DS1XG_CTRL_REG8, 0x44);

            is_sw_reset_performed = true;
        }
    }
}

bool Lsm9ds1HAL::initSensor(SensorType s_type, uint8_t config)
{
    bool ret = false;

    if(is_sw_reset_performed  == false)
    {
        resetDevice();
    }
    else
    {
        if(s_type == SensorType::Accelerometer)
        {
            /* 
            * Enable Xen_XL -> Accelerometer’s X-axis output enable
            * Enable Yen_XL -> Accelerometer’s Y-axis output enable
            * Enable Zen_XL -> Accelerometer’s Z-axis output enable
            */
            writeByte(LSM9DS1XG_CTRL_REG5_XL, 0x38);

            writeByte(LSM9DS1XG_CTRL_REG6_XL, config);
            usleep(200);

            ret = true;
        }

        if(s_type == SensorType::Gyroscope)
        {
            /* 
            * Enable Xen_G -> Gyroscope’s pitch axis (X) output enable
            * Enable Yen_G -> Gyroscope’s pitch axis (>) output enable
            * Enable Zen_G -> Gyroscope’s pitch axis (Z) output enable
            */
            writeByte(LSM9DS1XG_CTRL_REG4, 0x38);

            writeByte(LSM9DS1XG_CTRL_REG1_G, config);
            usleep(200);
            ret = true;    
        }        
    }

    return ret;
}

void Lsm9ds1HAL::selfTestEnabled(void)
{
    writeByte(LSM9DS1XG_CTRL_REG10, 0x01);
}

void Lsm9ds1HAL::selfTestDisabled(void)
{
    writeByte(LSM9DS1XG_CTRL_REG10, 0x00);
}

void Lsm9ds1HAL::getRawData(SensorType s_type, SensorData16Bit *data)
{
    if(s_type == SensorType::Accelerometer)
    {
        readBytes(LSM9DS1XG_OUT_X_L_XL, 7, &readBuffer[0]);
    }
    if(s_type == SensorType::Gyroscope)
    {
        readBytes(LSM9DS1XG_OUT_X_L_G, 7, &readBuffer[0]);
    }
    
    data->x = (((int16_t)readBuffer[1]) << 8) | readBuffer[0];
    data->y = (((int16_t)readBuffer[3]) << 8) | readBuffer[2];
    data->z = (((int16_t)readBuffer[5]) << 8) | readBuffer[4];
}

void Lsm9ds1HAL::fifoModeEnabled(void)
{
    uint8_t tmp;

    if (is_fifo_enabled == false)
    {
        readByte(LSM9DS1XG_CTRL_REG9, &tmp);
        /* Enable FIFO_EN -> FIFO memory enable */
        writeByte(LSM9DS1XG_CTRL_REG9, tmp | 0x02);      
        /* Wait for change to take effect */
        usleep(50);
        /* 
        * Enable FMODE0 -> FIFO mode. Stops collecting data when FIFO is full.
        * Enable FTH0-FTH4 -> FIFO threshold level setting -> maximum
        */
        writeByte(LSM9DS1XG_FIFO_CTRL, 0x20 | 0x1F);
        /* Time to collect FIFO samples */
        usleep(1000);

        is_fifo_enabled = true;
    }
}

void Lsm9ds1HAL::fifoModeDisabled(void)
{
    uint8_t tmp;

    if (is_fifo_enabled == true)
    {
        tmp = readByte(LSM9DS1XG_CTRL_REG9, &tmp);
        /* Disable FIFO_EN -> FIFO memory enable */
        writeByte(LSM9DS1XG_CTRL_REG9, tmp & ~0x02);
        /* Wait for change to take effect */ 
        usleep(50);
        /*
        * Disable FMODE0-3 -> Bypass mode. FIFO turned off
        * Disable FTH0-FTH4
        */
        writeByte(LSM9DS1XG_FIFO_CTRL, 0x00);
        usleep(50);

        is_fifo_enabled = false;
    }
}

void Lsm9ds1HAL::getFifoMeanValue(SensorType s_type, SensorData32Bit *mean_data)
{
    uint8_t cnt, samples;
    SensorData16Bit data;

    if (is_fifo_enabled == true)
    {
        readByte(LSM9DS1XG_FIFO_SRC, &samples); // Read number of stored samples
        /* FSS5 = 1 -> FIFO full -> 32 unread samples */
        if(samples & 0x20)
        {
            samples = 32;
        }
        else
        {
            samples &= 0x1F;
        }

        /* Read the accel data stored in the FIFO */
        for(cnt = 0; cnt < samples ; cnt++) {

            getRawData(s_type, &data);

            mean_data->x += (int32_t) data.x;
            mean_data->y += (int32_t) data.y;
            mean_data->z += (int32_t) data.z;
        } 

        /* Calculate the average values */
        mean_data->x /= samples;
        mean_data->y /= samples;
        mean_data->z /= samples;
    }
}