

#include <Wire.h>

#include "LSM9DS1_Types.h"




class LSM9DS1Class {
  public:

    IMUSettings settings;

    float aBias[3], gBias[3], mBias[3]; // variables for the bias that we'll compenseate for
    int16_t aBiasRaw[3], gBiasRaw[3], mBiasRaw[3]; // variables for the raw bias
    //  variables for storing each IMU element
    int16_t ax, ay, az; //  readouts from acclerometer x, y, and z axes
    int16_t gx, gy, gz; //  readouts from gyroscope x, y, and z axes
    int16_t mx, my, mz; //  readouts from magnetometer x, y, and z axes
    int16_t temperature;  //  readouts of chip temperature


    LSM9DS1Class(TwoWire& wire);
    virtual ~LSM9DS1Class();

    int begin();
    void end();

    // Controls whether a FIFO is continuously filled, or a single reading is stored.
    // Defaults to one-shot.
    void setContinuousMode();
    void setOneShotMode();

    // Accelerometer
    virtual int readAcceleration(float& aX, float& aY, float& aZ); // Results are in G (earth gravity).
    virtual int accelerationAvailable(); // Number of samples in the FIFO.
    virtual float accelerationSampleRate(); // Sampling rate of the sensor.

    // Gyroscope
    virtual int readGyroscope(float& gX, float& gY, float& gZ); // Results are in degrees/second.
    virtual int gyroscopeAvailable(); // Number of samples in the FIFO.
    virtual float gyroscopeSampleRate(); // Sampling rate of the sensor.

    // Magnetometer
    virtual int readMagneticField(float& mX, float& mY, float& mZ); // Results are in uT (micro Tesla).
    virtual int magneticFieldAvailable(); // Number of samples in the FIFO.
    virtual float magneticFieldSampleRate(); // Sampling rate of the sensor.
/*****************************************************/
    // readGyro() -- Read the gyroscope output registers. This function will read all six gyroscope output registers. The readings are stored in the class' gx, gy, and gz variables. Read  those _after_ calling readGyro().
    void readGyro();

    // readAccel() -- Read the accelerometer output registers. This function will read all six accelerometer output registers.  The readings are stored in the class' ax, ay, and az variables. Read those _after_ calling readAccel().
    void readAccel();

    // readMag() -- Read the magnetometer output registers. This function will read all six magnetometer output registers. The readings are stored in the class' mx, my, and mz variables. Read those _after_ calling readMag().
    void readMag();

    // readTemp() -- Read the temperature output register. This function will read two temperature output registers.  The combined readings are stored in the class' temperature variables. Read those _after_ calling readTemp().
    void readTemp();

    // calcGyro() -- Convert from RAW signed 16-bit value to degrees per second This function reads in a signed 16-bit value and returns the scaled DPS. This function relies on gScale and gRes being correct.
    float calcGyro(int16_t gyro); // Input:	- gyro = A signed 16-bit raw reading from the gyroscope.

    // calcAccel() -- Convert from RAW signed 16-bit value to gravity (g's). This function reads in a signed 16-bit value and returns the scaled g's. This function relies on aScale and aRes being correct.
    float calcAccel(int16_t accel); // Input:	- accel = A signed 16-bit raw reading from the accelerometer.

	  // calcMag() -- Convert from RAW signed 16-bit value to Gauss (Gs)  This function reads in a signed 16-bit value and returns the scaled Gs. This function relies on mScale and mRes being correct.
	  float calcMag(int16_t mag);  // Input:	- mag = A signed 16-bit raw reading from the magnetometer.
/*****************************************************/



  private:
    bool continuousMode;
    int readRegister(uint8_t slaveAddress, uint8_t address);
    int readRegisters(uint8_t slaveAddress, uint8_t address, uint8_t* data, size_t length);
    int writeRegister(uint8_t slaveAddress, uint8_t address, uint8_t value);

  private:
    TwoWire* _wire;

  protected:

    // init() -- Sets up gyro, accel, and mag settings to default. To set com interface and/or addresses see begin().
    void init();
    // _autoCalc keeps track of whether we're automatically subtracting off. accelerometer and gyroscope bias calculated in calibrate().
    bool _autoCalc;
    // Values for storing current resolution for the sensors. Units of these values would be DPS (or g's or Gs's) per ADC tick. This value is calculated as (sensor scale) / (2^15).
    float gRes, aRes, mRes;
    // calcgRes() -- Calculate the resolution of the gyroscope. This function will set the value of the gRes variable. gScale must be set prior to calling this function.
	  void calcgRes();
	  // calcmRes() -- Calculate the resolution of the magnetometer. This function will set the value of the mRes variable. mScale must be set prior to calling this function.
	  void calcmRes();
	  // calcaRes() -- Calculate the resolution of the accelerometer. This function will set the value of the aRes variable. aScale must be set prior to calling this function.
	  void calcaRes();
    // initAccel() -- Sets up the accelerometer to begin reading. This function steps through all accelerometer related control registers. Upon exit these registers will be set as:
	//	- CTRL_REG0_XM = 0x00: FIFO disabled. HPF bypassed. Normal mode.
	//	- CTRL_REG1_XM = 0x57: 100 Hz data rate. Continuous update. all axes enabled.
	//	- CTRL_REG2_XM = 0x00:  2g scale. 773 Hz anti-alias filter BW.
	//	- CTRL_REG3_XM = 0x04: Accel data ready signal on INT1_XM pin.
	void initAccel();
  // initGyro() -- Sets up the gyroscope to begin reading. This function steps through all five gyroscope control registers. Upon exit, the following parameters will be set:
	//	- CTRL_REG1_G = 0x0F: Normal operation mode, all axes enabled. 		95 Hz ODR, 12.5 Hz cutoff frequency.
	//	- CTRL_REG2_G = 0x00: HPF set to normal mode, cutoff frequency		set to 7.2 Hz (depends on ODR).
	//	- CTRL_REG3_G = 0x88: Interrupt enabled on INT_G (set to push-pull and active high). Data-ready output enabled on DRDY_G.
	//	- CTRL_REG4_G = 0x00: Continuous update mode. Data LSB stored in lower address. Scale set to 245 DPS. SPI mode set to 4-wire.
	//	- CTRL_REG5_G = 0x00: FIFO disabled. HPF disabled.
	void initGyro();
  // initMag() -- Sets up the magnetometer to begin reading. This function steps through all magnetometer-related control registers. Upon exit these registers will be set as:
	//	- CTRL_REG4_XM = 0x04: Mag data ready signal on INT2_XM pin.
	//	- CTRL_REG5_XM = 0x14: 100 Hz update rate. Low resolution. Interrupt requests don't latch. Temperature sensor disabled.
	//	- CTRL_REG6_XM = 0x00:  2 Gs scale.
	//	- CTRL_REG7_XM = 0x00: Continuous conversion mode. Normal HPF mode.
	//	- INT_CTRL_REG_M = 0x09: Interrupt active-high. Enable interrupts.
	void initMag();
};

extern LSM9DS1Class IMU;













//EOF
