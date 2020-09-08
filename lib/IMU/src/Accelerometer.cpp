#include "Accelerometer.h"
#include "Arduino.h"

bool Accelerometer::initialize(void) 
{
    Serial.println("Enter: Accelerometer::initialize");
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
            Serial.println("Exit: Accelerometer::initialize");
            return 1;
        }
        else
        {
            Serial.println("Exit: Accelerometer::initialize");
            return 0;
        }
    }
    else
    {
        Serial.println("Exit: Accelerometer::initialize");
        return 1;
    }
    
}

SensorData Accelerometer::getAcceleration(void)
{
    Serial.println("Enter: Accelerometer::getAcceleration");
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
    Serial.println("Exit: Accelerometer::getAcceleration");
    return acc;
}

void Accelerometer::getRawData(int16_t *data)
{
    Serial.println("Enter: Accelerometer::getRawData");
    uint8_t buffer[6];

    readBytes(LSM9DS1XG_OUT_X_L_XL, 7, &buffer[0]);
    data[0] = (((int16_t)buffer[1]) << 8) | buffer[0];
    data[1] = (((int16_t)buffer[3]) << 8) | buffer[2];
    data[2] = (((int16_t)buffer[5]) << 8) | buffer[4];
    Serial.println("Exit: Accelerometer::getRawData");
}

SensorData Accelerometer::selfTest(void)
{
    Serial.println("Enter: Accelerometer::selfTest");
    SensorData acc;
    float st[3] = {0., 0., 0.}; 
    float no_st[3] = {0., 0., 0.};

    writeByte(LSM9DS1XG_CTRL_REG10, 0x00); // disable self test
    getBias(&no_st[0]);
    
    /*writeByte(LSM9DS1XG_CTRL_REG10, 0x01); // enable accel self test
    
    getBias(&st[0]);
    
    acc.x = 1000.*((st[0] - no_st[0]));
    acc.y = 1000.*((st[1] - no_st[1]));
    acc.z = 1000.*((st[2] - no_st[2]));
    */
    Serial.println("Exit: Accelerometer::selfTest");
    return acc;
}

void Accelerometer::getBias(float * data)
{
  Serial.println("Enter: Accelerometer::getBias");
  int32_t bias[3] = {0, 0, 0};
  uint8_t cnt;

  enableFifoMode();
  
  getFifoMeanValue(&bias[0]);

  /* Remove gravity from the z-axis accelerometer bias calculation */
  if(bias[2] > 0L) 
  {
    bias[2] -= (int32_t) (1.0/resolution);
  }  
  else 
  {
    bias[2] += (int32_t) (1.0/resolution);
  }

  /* Properly scale the data to get g */
  for (cnt = 0; cnt < 3; cnt++){
    data[cnt] = (float)bias[cnt]*resolution;
  }

  disableFifoMode();
  Serial.println("Exit: Accelerometer::getBias");
}

void Accelerometer::enableFifoMode(void)
{
  Serial.println("Enter: Accelerometer::enableFifoMode");
  uint8_t tmp;
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
  Serial.println("Exit: Accelerometer::enableFifoMode");
}

void Accelerometer::disableFifoMode(void)
{
  Serial.println("Enter: Accelerometer::disableFifoMode");
  uint8_t tmp;
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
  Serial.println("Exit: Accelerometer::disableFifoMode");
}

void Accelerometer::getFifoMeanValue(int32_t * data)
{
  Serial.println("Enter: Accelerometer::getFifoMeanValue");
  uint8_t samples;
  uint8_t cnt, cnt2;

  readByte(LSM9DS1XG_FIFO_SRC, &samples); // Read number of stored samples
  //Serial.print("fifo samples raw:"); Serial.println(samples);
  /* FSS5 = 1 -> FIFO full -> 32 unread samples */
  if(samples & 0x20)
  {
      samples = 32;
  }
  else
  {
      samples = samples & 0x1F;
  }
  //Serial.print("fifo samples converted:"); Serial.println(samples);
  /* Read the accel data stored in the FIFO */
  for(cnt = 0; cnt < samples ; cnt++) {
    int16_t rawData[3];
    getRawData(&rawData[0]);

    /* Sum individual signed 16-bit biases to get accumulated signed 32-bit biases */
    for (cnt2 = 0; cnt2 < 3; cnt2++){
        data[cnt] += (int32_t) rawData[cnt];
    }
  } 
  
  /* Calculate the average values */
  for (cnt = 0; cnt < 3; cnt++){
    //Serial.print("raw acc summ"); Serial.print(cnt); Serial.print(" :"); Serial.println(data[cnt]);
    data[cnt] /= samples;
    //Serial.print("raw acc average value"); Serial.print(cnt); Serial.print(" :"); Serial.println(data[cnt]);
  }
  Serial.println("Exit: Accelerometer::getFifoMeanValue");
}
