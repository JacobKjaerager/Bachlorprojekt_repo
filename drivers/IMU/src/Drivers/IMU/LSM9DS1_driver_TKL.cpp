

#include "LSM9DS1_driver_TKL.h"



//
#define LSM9DS1_ADDRESS            0x6b							/**<	Physical address of the IMU IC	*/
#define LSM9DS1_CTRL_REG6_XL       0x20							/**<	Accelerometer control register	*/
#define LSM9DS1_CTRL_REG1_G        0x10							/**<	Gyroscope control register	*/
#define LSM9DS1_CTRL_REG8          0x22							/**<	Control register	*/
#define LSM9DS1_WHO_AM_I           0x0f							/**<	Address register	*/
#define LSM9DS1_STATUS_REG         0x17							/**<	Gyroscope and Accelerometer Status register	*/
//
// magnetometer
#define LSM9DS1_ADDRESS_M          0x1e							/**<	Magnetometer address*/
#define LSM9DS1_CTRL_REG2_M        0x21							/**<	Magnetometer control register*/
#define LSM9DS1_CTRL_REG3_M        0x22							/**<	Magnetometer control register*/
#define LSM9DS1_STATUS_REG_M       0x27							/**<	Magnetometer status register*/



// #define LSM9DS1_OUT_X_G            0x18
// #define LSM9DS1_OUT_X_XL           0x28

//
// #define LSM9DS1_CTRL_REG1_M        0x20
// #define LSM9DS1_OUT_X_L_M          0x28

#define SENSITIVITY_ACCELEROMETER_2   0.000061			/**<	used in calculations. Sensitivity factors	*/
#define SENSITIVITY_ACCELEROMETER_4   0.000122			/**<	used in calculations. Sensitivity factors	*/
#define SENSITIVITY_ACCELEROMETER_8   0.000244			/**<	used in calculations. Sensitivity factors	*/
#define SENSITIVITY_ACCELEROMETER_16  0.000732			/**<	used in calculations. Sensitivity factors	*/
#define SENSITIVITY_GYROSCOPE_245     0.008750			/**<	used in calculations. Sensitivity factors	*/
#define SENSITIVITY_GYROSCOPE_500     0.017500			/**<	used in calculations. Sensitivity factors	*/
#define SENSITIVITY_GYROSCOPE_2000    0.070000			/**<	used in calculations. Sensitivity factors	*/
#define SENSITIVITY_MAGNETOMETER_4    0.000014			/**<	used in calculations. Sensitivity factors	*/
#define SENSITIVITY_MAGNETOMETER_8    0.000029			/**<	used in calculations. Sensitivity factors	*/
#define SENSITIVITY_MAGNETOMETER_12   0.000043			/**<	used in calculations. Sensitivity factors	*/
#define SENSITIVITY_MAGNETOMETER_16   0.000058			/**<	used in calculations. Sensitivity factors	*/



enum lsm9ds1_axis {
	X_AXIS,	/**<	enum axis	*/
	Y_AXIS,	/**<	enum axis	*/
	Z_AXIS, /**<	enum axis	*/
	ALL_AXIS /**<	enum all axes	*/
};

LSM9DS1Class::LSM9DS1Class(TwoWire& wire) : continuousMode(false), _wire(&wire) /**< Constructor, with I2C object */
{}


LSM9DS1Class::~LSM9DS1Class() /**< Constructor */
{}


void LSM9DS1Class::init(){
  //  GYRO SETTINGS
  settings.gyro.enabled = true;					/**<	bool used to indicate if Gyroscope is enabled		*/
  settings.gyro.enableX = true;					/**<	bool used to indicate if axis is enabled		*/
  settings.gyro.enableY = true;					/**<	bool used to indicate if axis is enabled		*/
  settings.gyro.enableZ = true;					/**<	bool used to indicate if axis is enabled		*/
  settings.gyro.scale = 2000;						/**<  setting scale (245, 500 or 2000 degrees / second)*/
  settings.gyro.sampleRate = 3;					/**<  Setting samplerate for gyro (by using 1-6 corresponding to 14.9, 59.5, 119, 238, 476 or 952 Hz)*/
  settings.gyro.bandwidth = -1;					/**<  Setting cutoff frequency (using 0-3 - actual frequency depends on sample rate)		*/
  settings.gyro.lowPowerEnable = false;	/**<  Low power mode active: true / false 		*/
  settings.gyro.HPFEnable = false;			/**<	Highpass filter enabled: true / false		*/
  settings.gyro.HPFCutoff = 0;					/**<  Gyro high pass filter cutoff frequency: value between 0-9. Actual value depends on sample rate.		*/
  settings.gyro.flipX = false;					/**<  Is X axis is flipped. Depends on how the IMU is worn on the persons body: boolean		*/
	settings.gyro.flipY = false;					/**<  Is Y axis is flipped. Depends on how the IMU is worn on the persons body: boolean		*/
	settings.gyro.flipZ = false;					/**<  Is Z axis is flipped. Depends on how the IMU is worn on the persons body: boolean		*/
	settings.gyro.orientation = 0;				/**<  Meaning initial orientation is expected to the same as the IMU's baseline		*/
	settings.gyro.latchInterrupt = true;	/**<	Is Gyroscope prepped for activating interrupt.		*/


  //  ACC SETTINGS
  settings.accel.enabled = true;				/**<	bool used to indicate if Accelerometer is enabled	*/
  settings.accel.enableX = true;				/**<	bool used to indicate if axis is enabled		*/
  settings.accel.enableY = true;				/**<	bool used to indicate if axis is enabled		*/
  settings.accel.enableZ = true;				/**<	bool used to indicate if axis is enabled		*/
  settings.accel.scale = 2;							/**<	setting scale (2, 4, 8 or 16 g's ) (Lower is finer resolution)		*/
  settings.accel.sampleRate = 1;				/**<	Setting samplerate (by using 1-6 corresponding to 10, 50, 119, 238, 476 or 952 Hz)		*/
  settings.accel.bandwidth = -1;				/**<	Setting cutoff frequency (using 0-3 - actual frequency depends on sample rate)		*/
  settings.accel.highResEnable = false;	/**<	High resolution enabled: boolean		*/
  settings.accel.highResBandwidth = 0;	/**<	Setting high resolution bandwith (usint 0-3. setting ODR/50, ODR/9, ODR/100, ODR/400 )		*/


  //  MAG SETTINGS
  settings.mag.enabled = true;		  					/**<	Enabling mag on all axes		*/
  settings.mag.scale = 1;											/**<	setting scale (2, 4, 8 or 16 g's ) (Lower is finer resolution)		*/
  settings.mag.sampleRate = 8;								/**<	Setting samplerate (by using 1-6 corresponding to 0.625, 1.25, 2.5, 5, 10, 20, 40, 80 Hz)		*/
  settings.mag.tempCompensationEnable = true;	/**<	Compensate for temperature.		*/
  settings.mag.XYPerformance = 1;							/**<	Performance of magnetometer (using 0-3, designating: low power mode, medium performance, high performance, ultra high performance )		*/
  settings.mag.ZPerformance = 1;
  settings.mag.lowPowerEnable = false;				/**<	Low power mode: boolean		*/
  settings.mag.operatingMode = 00;						/**<	Setting operation mode, (using 00, 01, 11 and 10 corresponding to: continuous conversion, single conversion and power down)		*/


  //  TEMP SETTINGS
  settings.temp.enabled = true;					/**<  Enabling temp - for use in correcting bias		*/
  for (int i = 0; i < 3; i++) {
    gBias[i] = 0;
    aBias[i] = 0;
    mBias[i] = 0;
    gBiasRaw[i] = 0;
    aBiasRaw[i] = 0;
    mBiasRaw[i] = 0;
  }

  settings.accel._autoCalc = true;			/**<	Setting whether bias should be corrected used offset calculations	*/

}


int LSM9DS1Class::begin()
{
  _wire->begin();

  // reset
  writeRegister(LSM9DS1_ADDRESS, LSM9DS1_CTRL_REG8, 0x05);
  writeRegister(LSM9DS1_ADDRESS_M, LSM9DS1_CTRL_REG2_M, 0x0c);

  delay(10);
  settings.device.commInterface = IMU_MODE_I2C;
  init();

  Serial.println("init()\t\t\tDONE.");

  calcaRes();   //  Calculating the degrees per tick of the ADC
  calcgRes();   //  Calculating the g's per tick of the ADC
  calcmRes();   //  Calculating the g's per tick of the ADC
  Serial.println("calc*Res()\t\tDONE.");
  Serial.println("\n");

  if ((readRegister(LSM9DS1_ADDRESS, LSM9DS1_WHO_AM_I) != 0x68)) {
    end();
    return 0;
  }

  if ((readRegister(LSM9DS1_ADDRESS_M, LSM9DS1_WHO_AM_I) != 0x3d)) {
    end();
    return 0;
  }

  initAccel();  //  Setting enabled, preparing interrupts
  initGyro();
  initMag();

  return 1;
}

void LSM9DS1Class::setContinuousMode() {
  // Enable FIFO (see docs https://www.st.com/resource/en/datasheet/DM00103319.pdf)
  writeRegister(LSM9DS1_ADDRESS, 0x23, 0x02);
  // Set continuous mode
  writeRegister(LSM9DS1_ADDRESS, 0x2E, 0xC0);
  continuousMode = true;
}

void LSM9DS1Class::setOneShotMode() {
  // Disable FIFO (see docs https://www.st.com/resource/en/datasheet/DM00103319.pdf)
  writeRegister(LSM9DS1_ADDRESS, 0x23, 0x00);
  // Disable continuous mode
  writeRegister(LSM9DS1_ADDRESS, 0x2E, 0x00);

  continuousMode = false;
}

void LSM9DS1Class::end()
{
  writeRegister(LSM9DS1_ADDRESS_M, LSM9DS1_CTRL_REG3_M, 0x03);
  writeRegister(LSM9DS1_ADDRESS, LSM9DS1_CTRL_REG1_G, 0x00);
  writeRegister(LSM9DS1_ADDRESS, LSM9DS1_CTRL_REG6_XL, 0x00);

  _wire->end();
}



int LSM9DS1Class::accelerationAvailable()
{
  if (continuousMode) {
    // Read FIFO_SRC. If any of the rightmost 8 bits have a value, there is data.
    if (readRegister(LSM9DS1_ADDRESS, 0x2F) & 63) {
      return 1;
    }
  } else {
    if (readRegister(LSM9DS1_ADDRESS, LSM9DS1_STATUS_REG) & 0x01) {
      return 1;
    }
  }

  return 0;
}

float LSM9DS1Class::accelerationSampleRate()
{
  return settings.accel.sampleRate;
}



int LSM9DS1Class::gyroscopeAvailable()
{
  if (readRegister(LSM9DS1_ADDRESS, LSM9DS1_STATUS_REG) & 0x02) {
    return 1;
  }

  return 0;
}

float LSM9DS1Class::gyroscopeSampleRate()
{
  return settings.gyro.sampleRate;
}



int LSM9DS1Class::magneticFieldAvailable()
{
  if (readRegister(LSM9DS1_ADDRESS_M, LSM9DS1_STATUS_REG_M) & 0x08) {
    return 1;
  }

  return 0;
}

float LSM9DS1Class::magneticFieldSampleRate()
{
  return settings.mag.sampleRate;
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

int LSM9DS1Class::readRegisters(uint8_t slaveAddress, uint8_t address, uint8_t* data, size_t length)
{
  _wire->beginTransmission(slaveAddress);
  _wire->write(0x80 | address);
  if (_wire->endTransmission(false) != 0) {
    return -1;
  }

  if (_wire->requestFrom(slaveAddress, length) != length) {
    return 0;
  }

  for (size_t i = 0; i < length; i++) {
    *data++ = _wire->read();
  }

  return 1;
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


void LSM9DS1Class::calcaRes(){
  switch (settings.accel.scale) {
    case 2:   aRes = SENSITIVITY_ACCELEROMETER_2;   break;
    case 4:   aRes = SENSITIVITY_ACCELEROMETER_4;   break;
    case 8:   aRes = SENSITIVITY_ACCELEROMETER_8;   break;
    case 16:  aRes = SENSITIVITY_ACCELEROMETER_16;  break;
    default: break;
  }
}

void LSM9DS1Class::calcgRes(){
  switch (settings.gyro.scale) {
    case 245:   gRes = SENSITIVITY_GYROSCOPE_245;   break;
    case 500:   gRes = SENSITIVITY_GYROSCOPE_500;   break;
    case 2000:  gRes = SENSITIVITY_GYROSCOPE_2000;  break;
  }
}

void LSM9DS1Class::calcmRes(){
  switch (settings.mag.scale) {
    case 4:   mRes = SENSITIVITY_MAGNETOMETER_4;   break;
    case 8:   mRes = SENSITIVITY_MAGNETOMETER_8;   break;
    case 12:  mRes = SENSITIVITY_MAGNETOMETER_12;  break;
    case 16:  mRes = SENSITIVITY_MAGNETOMETER_16;  break;
    default: break;
  }
}

void LSM9DS1Class::initAccel(){

  uint8_t tempRegValue = 0;

	if (settings.accel.enableZ) tempRegValue |= (1<<5);
	if (settings.accel.enableY) tempRegValue |= (1<<4);
	if (settings.accel.enableX) tempRegValue |= (1<<3);

  writeRegister(LSM9DS1_ADDRESS, CTRL_REG5_XL, tempRegValue);

	tempRegValue = 0;

	if (settings.accel.enabled)
	{
		tempRegValue |= (settings.accel.sampleRate & 0x07) << 5;
	}
	switch (settings.accel.scale)
	{
		case 4:
			tempRegValue |= (0x2 << 3);
			break;
		case 8:
			tempRegValue |= (0x3 << 3);
			break;
		case 16:
			tempRegValue |= (0x1 << 3);
			break;
	}
	if (settings.accel.bandwidth >= 0)
	{
		tempRegValue |= (1<<2);
		tempRegValue |= (settings.accel.bandwidth & 0x03);
	}
  writeRegister(LSM9DS1_ADDRESS, CTRL_REG6_XL, tempRegValue);

	tempRegValue = 0;
	if (settings.accel.highResEnable)
	{
		tempRegValue |= (1<<7); // Set HR bit
		tempRegValue |= (settings.accel.highResBandwidth & 0x3) << 5;
	}

  writeRegister(LSM9DS1_ADDRESS, CTRL_REG7_XL, tempRegValue);

}


void LSM9DS1Class::initGyro(){
	uint8_t tempRegValue = 0;

	if (settings.gyro.enabled)
	{
		tempRegValue = (settings.gyro.sampleRate & 0x07) << 5;
	}
	switch (settings.gyro.scale)
	{
		case 500:
			tempRegValue |= (0x1 << 3);
			break;
		case 2000:
			tempRegValue |= (0x3 << 3);
			break;
	}
	tempRegValue |= (settings.gyro.bandwidth & 0x3);

  writeRegister(LSM9DS1_ADDRESS, CTRL_REG1_G, tempRegValue);
  writeRegister(LSM9DS1_ADDRESS, CTRL_REG2_G, 0x00);

	tempRegValue = settings.gyro.lowPowerEnable ? (1<<7) : 0;
	if (settings.gyro.HPFEnable)
	{
		tempRegValue |= (1<<6) | (settings.gyro.HPFCutoff & 0x0F);
	}
	writeRegister(LSM9DS1_ADDRESS, CTRL_REG3_G, tempRegValue);

	tempRegValue = 0;
	if (settings.gyro.enableZ) tempRegValue |= (1<<5);
	if (settings.gyro.enableY) tempRegValue |= (1<<4);
	if (settings.gyro.enableX) tempRegValue |= (1<<3);
	if (settings.gyro.latchInterrupt) tempRegValue |= (1<<1);
	writeRegister(LSM9DS1_ADDRESS, CTRL_REG4, tempRegValue);

	tempRegValue = 0;
	if (settings.gyro.flipX) tempRegValue |= (1<<5);
	if (settings.gyro.flipY) tempRegValue |= (1<<4);
	if (settings.gyro.flipZ) tempRegValue |= (1<<3);
	writeRegister(LSM9DS1_ADDRESS, ORIENT_CFG_G, tempRegValue);
}

void LSM9DS1Class::initMag(){
	uint8_t tempRegValue = 0;

	if (settings.mag.tempCompensationEnable) tempRegValue |= (1<<7);
	tempRegValue |= (settings.mag.XYPerformance & 0x3) << 5;
	tempRegValue |= (settings.mag.sampleRate & 0x7) << 2;
	writeRegister(LSM9DS1_ADDRESS_M, CTRL_REG1_M, tempRegValue);
  Serial.printf("CTRL_REG1_M:\t%d\n", tempRegValue );

	tempRegValue = 0;
	switch (settings.mag.scale)
	{
	case 8:
		tempRegValue |= (0x1 << 5);
		break;
	case 12:
		tempRegValue |= (0x2 << 5);
		break;
	case 16:
		tempRegValue |= (0x3 << 5);
		break;

	}
	writeRegister(LSM9DS1_ADDRESS_M, CTRL_REG2_M, tempRegValue);
  Serial.printf("CTRL_REG2_M:\t%d\n", tempRegValue );

	tempRegValue = 0;
	if (settings.mag.lowPowerEnable) tempRegValue |= (1<<5);
	tempRegValue |= (settings.mag.operatingMode & 0x3);
	writeRegister(LSM9DS1_ADDRESS_M, CTRL_REG3_M, tempRegValue);
  Serial.printf("CTRL_REG3_M:\t%d\n", tempRegValue );

	tempRegValue = 0;
	tempRegValue = (settings.mag.ZPerformance & 0x3) << 2;
	writeRegister(LSM9DS1_ADDRESS_M, CTRL_REG4_M, tempRegValue);
  Serial.printf("CTRL_REG4_M:\t%d\n", tempRegValue );

	tempRegValue = 0;
	writeRegister(LSM9DS1_ADDRESS_M, CTRL_REG5_M, tempRegValue);
  Serial.printf("CTRL_REG5_M:\t%d\n", tempRegValue );

}





void LSM9DS1Class::readAccel(){


	uint8_t temp[6];
	if ( readRegisters(LSM9DS1_ADDRESS, OUT_X_L_XL, temp, 6) /* == 6 */ )
	{

		ax = (temp[1] << 8) | temp[0];
		ay = (temp[3] << 8) | temp[2];
		az = (temp[5] << 8) | temp[4];
		if (settings.accel._autoCalc)
		{
			ax -= aBiasRaw[X_AXIS];
			ay -= aBiasRaw[Y_AXIS];
			az -= aBiasRaw[Z_AXIS];
		}
	}
}

void LSM9DS1Class::readMag(){
	uint8_t temp[6];
	if ( readRegisters(LSM9DS1_ADDRESS_M, OUT_X_L_M, temp, 6) /*== 6*/)
	{
		mx = (temp[1] << 8) | temp[0];
		my = (temp[3] << 8) | temp[2];
		mz = (temp[5] << 8) | temp[4];
	}
}

void LSM9DS1Class::readTemp(){
	uint8_t temp[2];
	if ( readRegisters(LSM9DS1_ADDRESS, OUT_TEMP_L, temp, 2) /*== 2*/ )
	{
		int16_t offset = 25;
		temperature = offset + ((((int16_t)temp[1] << 8) | temp[0]) >> 8) ;
	}
}

void LSM9DS1Class::readGyro(){
	uint8_t temp[6];
	if ( readRegisters(LSM9DS1_ADDRESS, OUT_X_L_G, temp, 6) /*== 6*/)
	{
		gx = (temp[1] << 8) | temp[0];
		gy = (temp[3] << 8) | temp[2];
		gz = (temp[5] << 8) | temp[4];
		if (settings.accel._autoCalc)
		{
			gx -= gBiasRaw[X_AXIS];
			gy -= gBiasRaw[Y_AXIS];
			gz -= gBiasRaw[Z_AXIS];
		}
	}
}

float LSM9DS1Class::calcGyro(int16_t gyro){

	return gRes * gyro;
}

float LSM9DS1Class::calcAccel(int16_t accel){

	return aRes * accel;
}

float LSM9DS1Class::calcMag(int16_t mag){

	return mRes * mag;
}

void LSM9DS1Class::enableFIFO(bool enable)
{
	// uint8_t temp = readRegister(LSM9DS1_ADDRESS, CTRL_REG9);
	// if (enable) temp |= (1<<1);
	// else temp &= ~(1<<1);
	// writeRegister(LSM9DS1_ADDRESS, CTRL_REG9, temp);

	// uint8_t temp = readRegister(LSM9DS1_ADDRESS, CTRL_REG9);
	// if (enable){
	// 	temp = 0x02;
	// }
	// else {
	// 	temp = 0x00;
	// }
	// writeRegister(LSM9DS1_ADDRESS, CTRL_REG9, temp);

	writeRegister(LSM9DS1_ADDRESS, 0x23, 0x02);
	writeRegister(LSM9DS1_ADDRESS, 0x2E, 0xC0);


}

void LSM9DS1Class::setFIFO(fifoMode_type fifoMode, uint8_t fifoThs)
{
	// Limit threshold - 0x1F (31) is the maximum. If more than that was asked
	// limit it to the maximum.
	uint8_t threshold = fifoThs <= 0x1F ? fifoThs : 0x1F;
	writeRegister(LSM9DS1_ADDRESS, FIFO_CTRL, ((fifoMode & 0x7) << 5) | (threshold & 0x1F));
}


void LSM9DS1Class::calibrate(bool autoCalc)
{
	uint8_t samples = 0;
	int ii;
	int32_t aBiasRawTemp[3] = {0, 0, 0};
	int32_t gBiasRawTemp[3] = {0, 0, 0};

	// Turn on FIFO and set threshold to 32 samples
	enableFIFO(true);
	setFIFO(FIFO_THS, 0x1F);
	// Serial.println("Reading samples");
	while (samples < 0x1F)
	{

		samples = (readRegister(LSM9DS1_ADDRESS, (FIFO_SRC) & 0x3F));// Read number of stored samples
		// samples = (readRegister(LSM9DS1_ADDRESS, (0x2F) & 0x3F));// Read number of stored samples
	}
	// Serial.println("Reading samples DONE");
	for(ii = 0; ii < samples ; ii++)
	{	// Read the gyro data stored in the FIFO
		// Serial.println("Reading Gyro");
		readGyro();
		// Serial.println("adding Gyro data to array ");
		gBiasRawTemp[0] += gx;
		gBiasRawTemp[1] += gy;
		gBiasRawTemp[2] += gz;
		// Serial.println("Reading Acc");
		readAccel();
		// Serial.println("adding Acc data to array ");
		aBiasRawTemp[0] += ax;
		aBiasRawTemp[1] += ay;
		aBiasRawTemp[2] += az - (int16_t)(1./aRes); // Assumes sensor facing up - meaning Z- axis is perpendicular to the ground
	}
	for (ii = 0; ii < 3; ii++)
	{
		gBiasRaw[ii] = gBiasRawTemp[ii] / samples;
		gBias[ii] = calcGyro(gBiasRaw[ii]);
		aBiasRaw[ii] = aBiasRawTemp[ii] / samples;
		aBias[ii] = calcAccel(aBiasRaw[ii]);
	}

	enableFIFO(false);
	setFIFO(FIFO_OFF, 0x00);

	// if (autoCalc) _autoCalc = true;
	if (autoCalc) settings.accel._autoCalc = true;
	Serial.println("Calibration done!");
}


#ifdef ARDUINO_ARDUINO_NANO33BLE
LSM9DS1Class IMU(Wire1);
#else
LSM9DS1Class IMU(Wire);
#endif













//EOF
