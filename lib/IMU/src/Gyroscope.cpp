#include "Gyroscope.h"

bool Gyroscope::initialize(void) 
{
    bool ret;
    uint8_t data;

    hal = &Lsm9ds1HAL::getInstance();

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

    ret = hal->initSensor(SensorType::Gyroscope, data);

    if(ret == true)
    {
    is_initialized = true;
    }
    else
    {
    is_initialized = false;
    }

    return ret;
}