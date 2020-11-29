#include "Accelerometer.h"

bool Accelerometer::initialize(void) 
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

  ret = hal->initSensor(SensorType::Accelerometer, data);

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

SensorDataFloat Accelerometer::getAcceleration(void)
{
  SensorDataFloat acc;

  if(is_initialized == true)
  {
      // Read raw measurement data
      SensorData16Bit data;
      hal->getRawData(SensorType::Accelerometer,&data);

      // Apply sensitivity adjustments, scale to get uT
      acc.x = data.x * resolution;
      acc.y = data.y * resolution;
      acc.z = data.z * resolution;
  }

  return acc;
}

SensorDataFloat Accelerometer::selfTest(void)
{
  SensorDataFloat acc;
  SensorDataFloat biasNoSelfTest;
  SensorDataFloat biasSelfTest;

  if(is_initialized == true)
  {
    hal->selfTestDisabled();
    getBias(&biasNoSelfTest);
    
    hal->selfTestEnabled();
    getBias(&biasSelfTest);

    acc.x = 1000.*(biasSelfTest.x - biasNoSelfTest.x);
    acc.y = 1000.*(biasSelfTest.y - biasNoSelfTest.y);
    acc.z = 1000.*(biasSelfTest.z - biasNoSelfTest.z);
  }

  return acc;
}

void Accelerometer::getBias(SensorDataFloat *data)
{
  SensorData32Bit acc_mean_val;

  hal->fifoModeEnabled();
  
  hal->getFifoMeanValue(SensorType::Accelerometer, &acc_mean_val);

  /* Remove gravity from the z-axis accelerometer bias calculation */
  if(acc_mean_val.z > 0L) 
  {
    acc_mean_val.z -= (int32_t) (1.0/resolution);
  }  
  else 
  {
    acc_mean_val.z += (int32_t) (1.0/resolution);
  }

  data->x = acc_mean_val.x * resolution;
  data->y = acc_mean_val.y * resolution;
  data->z = acc_mean_val.z * resolution;

  hal->fifoModeDisabled();
}