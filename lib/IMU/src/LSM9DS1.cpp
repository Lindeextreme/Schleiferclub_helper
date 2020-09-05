#include "LSM9DS1.h"
static const uint8_t LSM9DS1XG_ADDRESS          = 0x6B;  //  Address of accelerometer and gyrscope
static const uint8_t LSM9DS1M_ADDRESS           = 0x1E;  //  Address of magnetometer

// See also LSM9DS1 Register Map and Descriptions, http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00103319.pdf 
//
// Accelerometer and Gyrscope registers
static const uint8_t LSM9DS1XG_ACT_THS          = 0x04;
static const uint8_t LSM9DS1XG_ACT_DUR          = 0x05;
static const uint8_t LSM9DS1XG_INT_GEN_CFG_XL   = 0x06;
static const uint8_t LSM9DS1XG_INT_GEN_THS_X_XL = 0x07;
static const uint8_t LSM9DS1XG_INT_GEN_THS_Y_XL = 0x08;
static const uint8_t LSM9DS1XG_INT_GEN_THS_Z_XL = 0x09;
static const uint8_t LSM9DS1XG_INT_GEN_DUR_XL   = 0x0A;
static const uint8_t LSM9DS1XG_REFERENCE_G      = 0x0B;
static const uint8_t LSM9DS1XG_INT1_CTRL        = 0x0C;
static const uint8_t LSM9DS1XG_INT2_CTRL        = 0x0D;
static const uint8_t LSM9DS1XG_WHO_AM_I         = 0x0F; // should return = 0x68
static const uint8_t LSM9DS1XG_CTRL_REG1_G      = 0x10;
static const uint8_t LSM9DS1XG_CTRL_REG2_G      = 0x11;
static const uint8_t LSM9DS1XG_CTRL_REG3_G      = 0x12;
static const uint8_t LSM9DS1XG_ORIENT_CFG_G     = 0x13;
static const uint8_t LSM9DS1XG_INT_GEN_SRC_G    = 0x14;
static const uint8_t LSM9DS1XG_OUT_TEMP_L       = 0x15;
static const uint8_t LSM9DS1XG_OUT_TEMP_H       = 0x16;
static const uint8_t LSM9DS1XG_STATUS_REG       = 0x17;
static const uint8_t LSM9DS1XG_OUT_X_L_G        = 0x18;
static const uint8_t LSM9DS1XG_OUT_X_H_G        = 0x19;
static const uint8_t LSM9DS1XG_OUT_Y_L_G        = 0x1A;
static const uint8_t LSM9DS1XG_OUT_Y_H_G        = 0x1B;
static const uint8_t LSM9DS1XG_OUT_Z_L_G        = 0x1C;
static const uint8_t LSM9DS1XG_OUT_Z_H_G        = 0x1D;
static const uint8_t LSM9DS1XG_CTRL_REG4        = 0x1E;
static const uint8_t LSM9DS1XG_CTRL_REG5_XL     = 0x1F;
static const uint8_t LSM9DS1XG_CTRL_REG6_XL     = 0x20;
static const uint8_t LSM9DS1XG_CTRL_REG7_XL     = 0x21;
static const uint8_t LSM9DS1XG_CTRL_REG8        = 0x22;
static const uint8_t LSM9DS1XG_CTRL_REG9        = 0x23;
static const uint8_t LSM9DS1XG_CTRL_REG10       = 0x24;
static const uint8_t LSM9DS1XG_INT_GEN_SRC_XL   = 0x26;
//static const uint8_t LSM9DS1XG_STATUS_REG       = 0x27 // duplicate of = 0x17!
static const uint8_t LSM9DS1XG_OUT_X_L_XL       = 0x28;
static const uint8_t LSM9DS1XG_OUT_X_H_XL       = 0x29;
static const uint8_t LSM9DS1XG_OUT_Y_L_XL       = 0x2A;
static const uint8_t LSM9DS1XG_OUT_Y_H_XL       = 0x2B;
static const uint8_t LSM9DS1XG_OUT_Z_L_XL       = 0x2C;
static const uint8_t LSM9DS1XG_OUT_Z_H_XL       = 0x2D;
static const uint8_t LSM9DS1XG_FIFO_CTRL        = 0x2E;
static const uint8_t LSM9DS1XG_FIFO_SRC         = 0x2F;
static const uint8_t LSM9DS1XG_INT_GEN_CFG_G    = 0x30;
static const uint8_t LSM9DS1XG_INT_GEN_THS_XH_G = 0x31;
static const uint8_t LSM9DS1XG_INT_GEN_THS_XL_G = 0x32;
static const uint8_t LSM9DS1XG_INT_GEN_THS_YH_G = 0x33;
static const uint8_t LSM9DS1XG_INT_GEN_THS_YL_G = 0x34;
static const uint8_t LSM9DS1XG_INT_GEN_THS_ZH_G = 0x35;
static const uint8_t LSM9DS1XG_INT_GEN_THS_ZL_G = 0x36;
static const uint8_t LSM9DS1XG_INT_GEN_DUR_G    = 0x37;
//// Magnetometer registers
static const uint8_t LSM9DS1M_OFFSET_X_REG_L_M  = 0x05;
static const uint8_t LSM9DS1M_OFFSET_X_REG_H_M  = 0x06;
static const uint8_t LSM9DS1M_OFFSET_Y_REG_L_M  = 0x07;
static const uint8_t LSM9DS1M_OFFSET_Y_REG_H_M  = 0x08;
static const uint8_t LSM9DS1M_OFFSET_Z_REG_L_M  = 0x09;
static const uint8_t LSM9DS1M_OFFSET_Z_REG_H_M  = 0x0A;
static const uint8_t LSM9DS1M_WHO_AM_I          = 0x0F; // should be = 0x3D
static const uint8_t LSM9DS1M_CTRL_REG1_M       = 0x20;
static const uint8_t LSM9DS1M_CTRL_REG2_M       = 0x21;
static const uint8_t LSM9DS1M_CTRL_REG3_M       = 0x22;
static const uint8_t LSM9DS1M_CTRL_REG4_M       = 0x23;
static const uint8_t LSM9DS1M_CTRL_REG5_M       = 0x24;
static const uint8_t LSM9DS1M_STATUS_REG_M      = 0x27;
static const uint8_t LSM9DS1M_OUT_X_L_M         = 0x28;
static const uint8_t LSM9DS1M_OUT_X_H_M         = 0x29;
static const uint8_t LSM9DS1M_OUT_Y_L_M         = 0x2A;
static const uint8_t LSM9DS1M_OUT_Y_H_M         = 0x2B;
static const uint8_t LSM9DS1M_OUT_Z_L_M         = 0x2C;
static const uint8_t LSM9DS1M_OUT_Z_H_M         = 0x2D;
static const uint8_t LSM9DS1M_INT_CFG_M         = 0x30;
static const uint8_t LSM9DS1M_INT_SRC_M         = 0x31;
static const uint8_t LSM9DS1M_INT_THS_L_M       = 0x32;
static const uint8_t LSM9DS1M_INT_THS_H_M       = 0x33;


LSM9DS1Class::LSM9DS1Class(TwoWire& wire) :
    _wire(&wire)
{
  switch(_acc_scale)
  {
    case Acc_scale_settings::AFS_2G:
      _acc_resolution = 2.0/32768.0;
      break;
    case Acc_scale_settings::AFS_16G:
      _acc_resolution = 16.0/32768.0;
      break;
    case Acc_scale_settings::AFS_4G:
      _acc_resolution = 4.0/32768.0;
      break;
    case Acc_scale_settings::AFS_8G:
      _acc_resolution = 8.0/32768.0;
      break;
  }

  switch(_gyr_scale)
  {
    case Gyr_scale_settings::GFS_245DPS:
      _gyr_resolution = 245.0/32768.0;
      break;
    case Gyr_scale_settings::GFS_500DPS:
      _gyr_resolution = 500.0/32768.0;
      break;
    case Gyr_scale_settings::GFS_2000DPS:
      _gyr_resolution = 2000.0/32768.0;
      break;
  }

  switch (_mag_scale)
  {
    case Mag_scale_settings::MFS_4G:
      _mag_resolution = 4.0/32768.0;
      break;
    case Mag_scale_settings::MFS_8G:
      _mag_resolution = 8.0/32768.0;
      break;
    case Mag_scale_settings::MFS_12G:
      _mag_resolution = 12.0/32768.0;
      break;
    case Mag_scale_settings::MFS_16G:
      _mag_resolution = 16.0/32768.0;
      break;
  }
}

LSM9DS1Class::~LSM9DS1Class()
{
}

int LSM9DS1Class::begin()
{
  _wire->begin();

  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG8, 0x05);
  writeRegister(LSM9DS1M_ADDRESS, LSM9DS1M_CTRL_REG2_M, 0x0c);

  return 1;
}

int LSM9DS1Class::statusAccGyr()
{
  byte tmp;
  int ret_val;

  // Read WHO_AM_I register of LSM9DS1 accel/gyr
  tmp = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_WHO_AM_I);  

  tmp == 0x68 ? ret_val = 1 : ret_val = 0;

  return ret_val;
}

int LSM9DS1Class::statusMag()
{
  byte tmp;
  int ret_val;

  // Read WHO_AM_I register of LSM9DS1 magnetometer
  tmp = readRegister(LSM9DS1M_ADDRESS, LSM9DS1M_WHO_AM_I);
  tmp == 0x3D ? ret_val = 1 : ret_val = 0;

  return ret_val;
}

void LSM9DS1Class::selfTestAcc()
{
  float acc_self_test_disabled[3] = {0., 0., 0.}; 
  float acc_self_test_enabled[3] = {0., 0., 0.};

  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG10, 0x00); // disable self test
  getAccBias(acc_self_test_disabled);

  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG10, 0x01); // enable accel self test
  getAccBias(acc_self_test_enabled);

  float accdx = 1000.*((acc_self_test_enabled[0] - acc_self_test_disabled[0]));
  float accdy = 1000.*((acc_self_test_enabled[1] - acc_self_test_disabled[1]));
  float accdz = 1000.*((acc_self_test_enabled[2] - acc_self_test_disabled[2]));

  Serial.println("Accelerometer self-test results: ");
  Serial.print("x-axis = "); Serial.print(accdx); Serial.print(" mg"); Serial.println(" should be between 60 and 1700 mg");
  Serial.print("y-axis = "); Serial.print(accdy); Serial.print(" mg"); Serial.println(" should be between 60 and 1700 mg");
  Serial.print("z-axis = "); Serial.print(accdz); Serial.print(" mg"); Serial.println(" should be between 60 and 1700 mg");

}

void LSM9DS1Class::selfTestGyr()
{
  float gyr_self_test_disabled[3] = {0., 0., 0.}; 
  float gyr_self_test_enabled[3] = {0., 0., 0.};

  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG10, 0x00); // disable self test
  getGyrBias(gyr_self_test_disabled);

  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG10, 0x04); // enable gyr self test
  getGyrBias(gyr_self_test_enabled);

  float gyrdx = (gyr_self_test_enabled[0] - gyr_self_test_disabled[0]);
  float gyrdy = (gyr_self_test_enabled[1] - gyr_self_test_disabled[1]);
  float gyrdz = (gyr_self_test_enabled[2] - gyr_self_test_disabled[2]);

  Serial.println("Gyro self-test results: ");
  Serial.print("x-axis = "); Serial.print(gyrdx); Serial.print(" dps"); Serial.println(" should be between 20 and 250 dps");
  Serial.print("y-axis = "); Serial.print(gyrdy); Serial.print(" dps"); Serial.println(" should be between 20 and 250 dps");
  Serial.print("z-axis = "); Serial.print(gyrdz); Serial.print(" dps"); Serial.println(" should be between 20 and 250 dps");
}
// Function which accumulates accelerometer data after device initialization. It calculates the average
// of the at-rest readings and then loads the resulting offsets into accelerometer bias registers.
void LSM9DS1Class::getAccBias(float * ret_val)
{
  int32_t acc_bias[3] = {0, 0, 0};
  uint8_t cnt;

  enableAccSensor();
  enableFifoMode();

  getGyrFifoMeanValue(&acc_bias[0]);  

  /* Remove gravity from the z-axis accelerometer bias calculation */
  if(acc_bias[2] > 0L) 
  {
    acc_bias[2] -= (int32_t) (1.0/_acc_resolution);
  }  
  else 
  {
    acc_bias[2] += (int32_t) (1.0/_acc_resolution);
  }

  /* Properly scale the data to get g */
  for (cnt = 0; cnt < 3; cnt++){
    ret_val[cnt] = (float)acc_bias[cnt]*_acc_resolution;
  }

  disableFifoMode();
  disableAccSensor();
}

// Function which accumulates gyr data after device initialization. It calculates the average
// of the at-rest readings and then loads the resulting offsets into gyr bias registers.
void LSM9DS1Class::getGyrBias(float *ret_val)
{
  int32_t gyr_bias[3] = {0, 0, 0};
  uint8_t cnt;

  enableGyrSensor();
  enableFifoMode();

  getGyrFifoMeanValue(&gyr_bias[0]);

  /* Properly scale the data to get g */
  for (cnt = 0; cnt < 3; cnt++){
    ret_val[cnt] = (float)gyr_bias[cnt]*_gyr_resolution;
  }

  disableFifoMode();
  disableGyrSensor();
}


void LSM9DS1Class::enableAccSensor(void)
{
  /* 
   * Enable Xen_G -> Gyroscope’s pitch axis (X) output enable
   * Enable Yen_G -> Gyroscope’s pitch axis (>) output enable
   * Enable Zen_G -> Gyroscope’s pitch axis (Z) output enable
   */
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG5_XL, 0x38);
  // configure the accelerometer-specify bandwidth selection with Abw
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG6_XL, static_cast<uint8_t>(_acc_sample_rate) << 5 | static_cast<uint8_t>(_acc_scale) << 3 | 0x04 | static_cast<uint8_t>(_acc_data_bandwith));
  delay(200);
  /*
   * Enable BDU -> Block data update -> output registers not updated until MSB and LSB read
   * Enable IF_ADD_INC -> Register address automatically incremented during a multiple byte access with a serial interface
   */ 
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG8, 0x44);  
}

void LSM9DS1Class::disableAccSensor(void)
{
  uint8_t tmp;

  tmp = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG5_XL);
  /* 
   * Disable Xen_G -> Gyroscope’s pitch axis (X) output enable
   * Disable Yen_G -> Gyroscope’s pitch axis (>) output enable
   * Disable Zen_G -> Gyroscope’s pitch axis (Z) output enable
   */
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG5_XL, tmp & ~0x38);
  // configure the accelerometer-specify bandwidth selection with Abw
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG6_XL, 0x0);
  delay(200);
  /*
   * Disable BDU -> Block data update -> output registers not updated until MSB and LSB read
   * Disable IF_ADD_INC -> Register address automatically incremented during a multiple byte access with a serial interface
   */ 
  tmp = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG8);
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG8, tmp & ~0x44);  
}

void LSM9DS1Class::enableGyrSensor(void)
{
  // enable the 3-axes of the gyrscope
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG4, 0x38);
  // configure the gyrscope
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG1_G, static_cast<uint8_t>(_gyr_sample_rate) << 5 | static_cast<uint8_t>(_gyr_scale) << 3 | static_cast<uint8_t>(_gyr_data_bandwith));
  delay(200);
  /*
   * Enable BDU -> Block data update -> output registers not updated until MSB and LSB read
   * Enable IF_ADD_INC -> Register address automatically incremented during a multiple byte access with a serial interface
   */ 
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG8, 0x44);
}

void LSM9DS1Class::disableGyrSensor(void)
{
  uint8_t tmp;

  tmp = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG4);
  // enable the 3-axes of the gyrscope
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG4, tmp & ~0x38);
  // configure the gyrscope
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG1_G, 0x0);
  delay(200);
  /*
   * Enable BDU -> Block data update -> output registers not updated until MSB and LSB read
   * Enable IF_ADD_INC -> Register address automatically incremented during a multiple byte access with a serial interface
   */ 
  tmp = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG8);
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG8, tmp & ~0x44);
}

void LSM9DS1Class::enableFifoMode(void)
{
  uint8_t tmp;
  tmp = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG9);
  /* Enable FIFO_EN -> FIFO memory enable */
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG9, tmp | 0x02);      
  /* Wait for change to take effect */
  delay(50);                                               
  /* 
   * Enable FMODE0 -> FIFO mode. Stops collecting data when FIFO is full.
   * Enable FTH0-FTH4 -> FIFO threshold level setting -> maximum
   */
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_FIFO_CTRL, 0x20 | 0x1F);
  /* Time to collect FIFO samples */
  delay(1000);
}

void LSM9DS1Class::getAccFifoMeanValue(int32_t * data)
{
  uint16_t samples;
  uint8_t cnt;

  samples = (readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_FIFO_SRC) & 0x2F); // Read number of stored samples

  /* Read the accel data stored in the FIFO */
  for(cnt = 0; cnt < samples ; cnt++) {
    SensorData tmp;
    getAccData(&tmp);

    /* Sum individual signed 16-bit biases to get accumulated signed 32-bit biases */
    data[0] += (int32_t) tmp.x;
    data[1] += (int32_t) tmp.y; 
    data[2] += (int32_t) tmp.z; 
  } 
  
  /* Calculate the average values */
  for (cnt = 0; cnt < 3; cnt++){
    data[cnt] /= samples;
  }
}

void LSM9DS1Class::getGyrFifoMeanValue(int32_t * data)
{
  uint16_t samples;
  uint8_t cnt;

  samples = (readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_FIFO_SRC) & 0x2F); // Read number of stored samples

  /* Read the accel data stored in the FIFO */
  for(cnt = 0; cnt < samples ; cnt++) {
    SensorData tmp;
    getAccData(&tmp);

    /* Sum individual signed 16-bit biases to get accumulated signed 32-bit biases */
    data[0] += (int32_t) tmp.x;
    data[1] += (int32_t) tmp.y; 
    data[2] += (int32_t) tmp.z; 
  } 
  
  /* Calculate the average values */
  for (cnt = 0; cnt < 3; cnt++){
    data[cnt] /= samples;
  }
}

void LSM9DS1Class::disableFifoMode(void)
{
  uint8_t tmp;
  tmp = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG9);
  /* Disable FIFO_EN -> FIFO memory enable */
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG9, tmp & ~0x02);
  /* Wait for change to take effect */ 
  delay(50);
  /*
   * Disable FMODE0-3 -> Bypass mode. FIFO turned off
   * Disable FTH0-FTH4
   */
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_FIFO_CTRL, 0x00);  // Enable gyro bypass mode
}

void LSM9DS1Class::getAccData(SensorData * data)
{
  uint8_t tmp_low;
  uint8_t tmp_high;
  
  /* Get x-axis */
  tmp_low = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_OUT_X_L_XL);
  tmp_high = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_OUT_X_H_XL);
  data->x = (int16_t) (((int16_t)tmp_high << 8) | tmp_low);

  /* Get y-axis */
  tmp_low = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_OUT_Y_L_XL);
  tmp_high = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_OUT_Y_H_XL);
  data->y = (int16_t) (((int16_t)tmp_high << 8) | tmp_low);

  /* Get z-axis */
  tmp_low = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_OUT_Z_L_XL);
  tmp_high = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_OUT_Z_H_XL);
  data->z = (int16_t) (((int16_t)tmp_high << 8) | tmp_low);
}

void LSM9DS1Class::getGyrData(SensorData * data)
{
  uint8_t tmp_low;
  uint8_t tmp_high;
  
  /* Get x-axis */
  tmp_low = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_OUT_X_L_G);
  tmp_high = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_OUT_X_H_G);
  data->x = (int16_t) (((int16_t)tmp_high << 8) | tmp_low);

  /* Get y-axis */
  tmp_low = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_OUT_Y_L_G);
  tmp_high = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_OUT_Y_H_G);
  data->y = (int16_t) (((int16_t)tmp_high << 8) | tmp_low);

  /* Get z-axis */
  tmp_low = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_OUT_Z_L_G);
  tmp_high = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_OUT_Z_H_G);
  data->z = (int16_t) (((int16_t)tmp_high << 8) | tmp_low);
}

void LSM9DS1Class::getMagData(SensorData * data)
{
  uint8_t tmp_low;
  uint8_t tmp_high;
  
  /* Get x-axis */
  tmp_low = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1M_OUT_X_L_M);
  tmp_high = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1M_OUT_X_H_M);
  data->x = (int16_t) (((int16_t)tmp_high << 8) | tmp_low);

  /* Get y-axis */
  tmp_low = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1M_OUT_Y_L_M);
  tmp_high = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1M_OUT_Y_H_M);
  data->y = (int16_t) (((int16_t)tmp_high << 8) | tmp_low);

  /* Get z-axis */
  tmp_low = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1M_OUT_Z_L_M);
  tmp_high = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1M_OUT_Z_H_M);
  data->z = (int16_t) (((int16_t)tmp_high << 8) | tmp_low);
}


