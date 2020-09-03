#include "LSM9DS1.h"
static const uint8_t LSM9DS1XG_ADDRESS          = 0x6B;  //  Address of accelerometer and gyroscope
static const uint8_t LSM9DS1M_ADDRESS           = 0x1E;  //  Address of magnetometer

// See also LSM9DS1 Register Map and Descriptions, http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00103319.pdf 
//
// Accelerometer and Gyroscope registers
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

  switch(_gyro_scale)
  {
    case Gyro_scale_settings::GFS_245DPS:
      _gyro_resolution = 245.0/32768.0;
      break;
    case Gyro_scale_settings::GFS_500DPS:
      _gyro_resolution = 500.0/32768.0;
      break;
    case Gyro_scale_settings::GFS_2000DPS:
      _gyro_resolution = 2000.0/32768.0;
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

int LSM9DS1Class::statusAccGyro()
{
  byte tmp;
  int ret_val;

  // Read WHO_AM_I register of LSM9DS1 accel/gyro
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
  calibrateAcc(acc_self_test_disabled);

  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG10, 0x05); // enable gyro/accel self test
  calibrateAcc(acc_self_test_enabled);
}

void LSM9DS1Class::selfTestGyro()
{
  float gyro_self_test_disabled[3] = {0., 0., 0.}; 
  float gyro_self_test_enabled[3] = {0., 0., 0.};

  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG10, 0x00); // disable self test
  calibrateGyro(gyro_self_test_disabled);

  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG10, 0x05); // enable gyro/accel self test
  calibrateGyro(gyro_self_test_enabled);
}
// Function which accumulates accelerometer data after device initialization. It calculates the average
// of the at-rest readings and then loads the resulting offsets into accelerometer bias registers.
void LSM9DS1Class::calibrateAcc(float * ret_val)
{
  uint8_t data[6] = {0, 0, 0, 0, 0, 0};
  int32_t accel_bias[3] = {0, 0, 0};
  uint16_t samples, ii;
  
  // enable the three axes of the accelerometer 
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG5_XL, 0x38);
  // configure the accelerometer-specify bandwidth selection with Abw
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG6_XL, static_cast<uint8_t>(_acc_sample_rate) << 5 | static_cast<uint8_t>(_acc_scale) << 3 | 0x04 | static_cast<uint8_t>(_acc_data_bandwith));
  delay(200);
  // enable block data update, allow auto-increment during multiple byte read
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG8, 0x44);

  // now get the accelerometer bias
  byte c = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG9);
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG9, c | 0x02);     // Enable accel FIFO  
  delay(50);                                                       // Wait for change to take effect
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_FIFO_CTRL, 0x20 | 0x1F);  // Enable accel FIFO stream mode and set watermark at 32 samples
  delay(1000);  // delay 1000 milliseconds to collect FIFO samples

  samples = (readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_FIFO_SRC) & 0x2F); // Read number of stored samples

  for(ii = 0; ii < samples ; ii++) {            // Read the accel data stored in the FIFO
    int16_t accel_temp[3] = {0, 0, 0};
    readBytes(LSM9DS1XG_ADDRESS, LSM9DS1XG_OUT_X_L_XL, 6, &data[0]);
    accel_temp[0] = (int16_t) (((int16_t)data[1] << 8) | data[0]); // Form signed 16-bit integer for each sample in FIFO
    accel_temp[1] = (int16_t) (((int16_t)data[3] << 8) | data[2]);
    accel_temp[2] = (int16_t) (((int16_t)data[5] << 8) | data[4]);

    accel_bias[0] += (int32_t) accel_temp[0]; // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
    accel_bias[1] += (int32_t) accel_temp[1]; 
    accel_bias[2] += (int32_t) accel_temp[2]; 
  }  

  accel_bias[0] /= samples; // average the data
  accel_bias[1] /= samples; 
  accel_bias[2] /= samples; 

  // Remove gravity from the z-axis accelerometer bias calculation
  if(accel_bias[2] > 0L) 
  {
    accel_bias[2] -= (int32_t) (1.0/_acc_resolution);
  }  
  else {accel_bias[2] += (int32_t) (1.0/_acc_resolution);}

  ret_val[0] = (float)accel_bias[0]*_acc_resolution;  // Properly scale the data to get g
  ret_val[1] = (float)accel_bias[1]*_acc_resolution;
  ret_val[2] = (float)accel_bias[2]*_acc_resolution;

  c = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG9);
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG9, c & ~0x02);   //Disable accel FIFO  
  delay(50);
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_FIFO_CTRL, 0x00);  // Enable accel bypass mode  
}

// Function which accumulates gyro data after device initialization. It calculates the average
// of the at-rest readings and then loads the resulting offsets into gyro bias registers.
void LSM9DS1Class::calibrateGyro(float *ret_val)
{
  uint8_t data[6] = {0, 0, 0, 0, 0, 0};
  int32_t gyro_bias[3] = {0, 0, 0};
  uint16_t samples, ii;

  // enable the 3-axes of the gyroscope
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG4, 0x38);
  // configure the gyroscope
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG1_G, static_cast<uint8_t>(_gyro_sample_rate) << 5 | static_cast<uint8_t>(_gyro_scale) << 3 | static_cast<uint8_t>(_gyro_data_bandwith));
  delay(200);
  // enable block data update, allow auto-increment during multiple byte read
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG8, 0x44);

  // now get gyro bias
  byte c = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG9);
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG9, c | 0x02);     // Enable gyro FIFO  
  delay(50);                                                       // Wait for change to take effect
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_FIFO_CTRL, 0x20 | 0x1F);  // Enable gyro FIFO stream mode and set watermark at 32 samples
  delay(1000);  // delay 1000 milliseconds to collect FIFO samples

  samples = (readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_FIFO_SRC) & 0x2F); // Read number of stored samples

  for(ii = 0; ii < samples ; ii++) {            // Read the gyro data stored in the FIFO
    int16_t gyro_temp[3] = {0, 0, 0};
    readBytes(LSM9DS1XG_ADDRESS, LSM9DS1XG_OUT_X_L_G, 6, &data[0]);
    gyro_temp[0] = (int16_t) (((int16_t)data[1] << 8) | data[0]); // Form signed 16-bit integer for each sample in FIFO
    gyro_temp[1] = (int16_t) (((int16_t)data[3] << 8) | data[2]);
    gyro_temp[2] = (int16_t) (((int16_t)data[5] << 8) | data[4]);

    gyro_bias[0] += (int32_t) gyro_temp[0]; // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
    gyro_bias[1] += (int32_t) gyro_temp[1]; 
    gyro_bias[2] += (int32_t) gyro_temp[2]; 
  }  

  gyro_bias[0] /= samples; // average the data
  gyro_bias[1] /= samples; 
  gyro_bias[2] /= samples; 

  ret_val[0] = (float)gyro_bias[0]*_gyro_resolution;  // Properly scale the data to get deg/s
  ret_val[1] = (float)gyro_bias[1]*_gyro_resolution;
  ret_val[2] = (float)gyro_bias[2]*_gyro_resolution;

  c = readRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG9);
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_CTRL_REG9, c & ~0x02);   //Disable gyro FIFO  
  delay(50);
  writeRegister(LSM9DS1XG_ADDRESS, LSM9DS1XG_FIFO_CTRL, 0x00);  // Enable gyro bypass mode
}

int LSM9DS1Class::readRegister(uint8_t slaveAddress, uint8_t address)
{
  _wire->beginTransmission(slaveAddress);
  _wire->write(address);
  if (_wire->endTransmission() != 0) {
    return -1;
  }

  if (_wire->requestFrom(slaveAddress, 1) != 1) {
    return -1;
  }

  return _wire->read();
}

void LSM9DS1Class::readBytes(uint8_t slaveAddress, uint8_t address, uint8_t count, uint8_t * dest)
{
  uint8_t i = 0;
  _wire->beginTransmission(slaveAddress);
  _wire->write(address);
  _wire->endTransmission(false);            // Send the Tx buffer, but send a restart to keep connection alive
  _wire->requestFrom(slaveAddress, count);  // Read bytes from slave register address 
  
  while (_wire->available()) {
    dest[i++] = _wire->read(); }         // Put read results in the Rx buffer
}

int LSM9DS1Class::writeRegister(uint8_t slaveAddress, uint8_t address, uint8_t value)
{
  _wire->beginTransmission(slaveAddress);
  _wire->write(address);
  _wire->write(value);
  if (_wire->endTransmission() != 0) {
    return 0;
  }

  return 1;
}
