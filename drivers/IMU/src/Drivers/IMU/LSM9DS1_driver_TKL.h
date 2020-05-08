
/**
  *
  *
  *	@file:		LSM9DS1_driver_TKL.h
  *	@date:		26-03-2020 09:32:41
  *	@author:	Tonni Lutze
  *
  *	@brief    Driver for the LSM9DS1 IMU on Arduino Nano 33 BLE / Sense
  *
  *
  *
  *
**/

#include <Wire.h>
#include "LSM9DS1_Types.h"

class LSM9DS1Class {
  public:

    IMUSettings settings;

    float aBias[3], gBias[3], mBias[3];             /**< variables for the calculated bias that we'll compenseate */
    int16_t aBiasRaw[3], gBiasRaw[3], mBiasRaw[3];  /**< variables for the raw bias values */
    //  variables for storing each IMU element
    int16_t ax, ay, az;   /**<  readouts from acclerometer x, y, and z axes  */
    int16_t gx, gy, gz;   /**<  readouts from gyroscope x, y, and z axes  */
    int16_t mx, my, mz;   /**<  readouts from magnetometer x, y, and z axes */
    int16_t temperature;  /**<  readouts of chip temperature  */

    LSM9DS1Class(TwoWire& wire);
    virtual ~LSM9DS1Class();

    /**
    * @brief  begin(): Initialization for the IMU
    *
    *   Initiates the IMU, and sets up the default values for the components
    *   sampletates, resolution as well as creating the I2C object that contains
    *   the connection to the IMU.
    *
    * @param None.
    * @return int: 1 = successfull init, 0 = failure.
    *
    **/
    int begin();

    /**
    * @brief  end(): de-initialization for the IMU - effectively a shutdown.
    *
    *   closes I2C connections and shuts down the IMU.
    *
    * @TODO  This is only used if the initialization fails at the moment - and maybe its not really necssessary.
    * @param None.
    * @return int: 1 = successfull init, 0 = failure.
    *
    **/
    void end();

    /**
    * @brief  calibrate(): Collecting samples for, and calculateing bias/offset
    *
    *   Used for calculating the needed offsets for the biases that are
    *   prescent in any sensor.
    *
    * @param bool autoCalc (default true): this sets whether future calculations on data from the IMU should be subject to corrections.
    * @return void
    *
    **/
    void calibrate(bool autoCalc = true);

    /**
    * @brief  enableFIFO(): Dis/enable the built-in FIFO buffer in the IMU
    *
    *   Dis/enable the built-in FIFO buffer in the IMU.
    *
    * @param bool enable: FIFO enabled / disabled
    * @return void
    *
    **/
    void enableFIFO(bool enable);

    /**
    * @brief  setFIFO(): Sets the FIFO mode.
    *
    *   Setting the mode and threshold used in triggering the FIRO to go into continious mode og into off mode.
    *
    * @param fifoMode_type fifoMode: The operation mode of the FIFO - See LSM9DS1 datasheet for possible modes
    * @param uint8_t fifoThs: The threshold for an eventual trigger
    * @return void
    *
    **/
    void setFIFO(fifoMode_type fifoMode, uint8_t fifoThs);

    /**
    * @brief  setContinuousMode(): Sets up the IMU to continuously fill the FIFO
    *
    *   Setting the mode of the FIFO to continuous.
    *   This fills the FIFO overwriting the oldest data.
    *
    * @param None.
    * @return void
    *
    **/
    void setContinuousMode();
    /**
    * @brief  setOneShotMode(): Sets up the IMU to only do a single readings.
    *
    *   Setting the mode of the FIFO to single read.
    *   This means that every poll on the IMU is answered with a single readout
    *   OneShotMode is default.
    *
    * @param None.
    * @return void
    *
    **/
    void setOneShotMode();

    /**
    * @brief  accelerationAvailable(): Polls IMU to see if data is ready.
    *
    *   Used to chech whether the IMU is ready to deliver a sample of data
    *   from the Accelerometer.
    *
    * @param None.
    * @return int: 1 = data ready, 0 = data NOT ready
    *
    **/
    virtual int accelerationAvailable(); // Number of samples in the FIFO.

    /**
    * @brief  accelerationSampleRate(): Reads and returns the current sample rate
    *
    *   Reads and returns the current sample rate from the IMUSettings,
    *   specifically the sample rate of the Accelerometer
    *   @NOTE if the Gyroscope is enabled, the sample rate of the Gyroscope is ALSO the samplerate of the Accelerometer.
    *
    * @param None.
    * @return int: 1 = data ready, 0 = data NOT ready
    *
    **/
    virtual float accelerationSampleRate(); // Sampling rate of the sensor.


    /**
    * @brief  gyroscopeAvailable(): Reads and returns the current sample rate
    *
    *   Reads and returns the current sample rate from the IMUSettings,
    *   specifically the sample rate of the Gyroscope
    *   @NOTE if the Gyroscope is enabled, the sample rate of the Gyroscope is ALSO the samplerate of the Accelerometer.
    *
    * @param None.
    * @return int: 1 = data ready, 0 = data NOT ready
    *
    **/
    virtual int gyroscopeAvailable();

    /**
    * @brief  gyroscopeSampleRate(): Polls IMU to see if data is ready.
    *
    *   Used to chech whether the IMU is ready to deliver a sample of data
    *   from the Gyroscope.
    *
    * @param None.
    * @return float: sample rate
    *
    **/
    virtual float gyroscopeSampleRate();

    /**
    * @brief  magneticFieldAvailable(): Polls IMU to see if data is ready.
    *
    *   Used to chech whether the IMU is ready to deliver a sample of data
    *   from the Magnetometer.
    *
    * @param None.
    * @return float: sample rate
    *
    **/
    virtual int magneticFieldAvailable(); // Number of samples in the FIFO.

    /**
    * @brief  magneticFieldSampleRate(): Reads and returns the current sample rate
    *
    *   Reads and returns the current sample rate from the IMUSettings,
    *   specifically the sample rate of the Magnetometer.
    *
    * @param None.
    * @return int: 1 = data ready, 0 = data NOT ready
    *
    **/
    virtual float magneticFieldSampleRate();
/*****************************************************/
    /**
    * @brief  readGyro(): Read the gyroscope output registers.
    *
    *   This function will read all six gyroscope output registers.
    *   The readings are stored in the class' gx, gy, and gz variables.
    *   Read  those _after_ calling readGyro().
    *
    * @param None.
    * @return void
    *
    **/
    void readGyro();

    /**
    * @brief  readAccel(): Read the accelerometer output registers.
    *
    *   This function will read all six accelerometer output registers.
    *   The readings are stored in the class' ax, ay, and az variables.
    *   Read those _after_ calling readAccel().
    *
    * @param None.
    * @return void
    *
    **/
    void readAccel();

    /**
    * @brief  readMag(): Read the magnetometer output registers.
    *
    *   This function will read all six magnetometer output registers.
    *   The readings are stored in the class' mx, my, and mz variables.
    *   Read those _after_ calling readMag().
    *
    * @param None.
    * @return void
    *
    **/
    void readMag();


    /**
    * @brief  readTemp(): Read the temperature output register.
    *
    *   This function will read two temperature output registers.
    *   The combined readings are stored in the class' temperature variables.
    *   Read those _after_ calling readTemp().
    *
    * @param None.
    * @return void
    *
    **/
    void readTemp();


    /**
    * @brief  calcGyro(): Converts the raw output from the Gyroscope of the IMU
    *
    *   Convert from RAW signed 16-bit value to degrees per second.
    *   This function reads in a signed 16-bit value and returns the scaled DPS.
    *   This function relies on gScale and gRes being correct.
    *
    * @param int16_t gyro: Raw 16 bit value from IMU Gyroscope Readout
    *
    * @return float: m/s offset
    *
    **/
    float calcGyro(int16_t gyro); // Input:	- gyro = A signed 16-bit raw reading from the gyroscope.

    /**
    * @brief  calcAccel(): Converts the raw output from the Accelerometer of the IMU
    *
    *   Convert from RAW signed 16-bit value to force in g.
    *   This function reads in a signed 16-bit value and returns the scaled g's.
    *   This function relies on aScale and aRes being correct.
    *
    * @param int16_t accel: Raw 16 bit value from IMU Accelerometer Readout
    *
    * @return float: g offset
    *
    **/
    float calcAccel(int16_t accel); // Input:	- accel = A signed 16-bit raw reading from the accelerometer.

    /**
    * @brief  calcMag(): Converts the raw output from the Magnetometer of the IMU
    *
    *   Convert from RAW signed 16-bit value to Gauss (Gs)
    *   This function reads in a signed 16-bit value and returns the scaled Gs.
    *   This function relies on mScale and mRes being correct.
    *
    * @param int16_t mag: Raw 16 bit value from IMU Magnetometer Readout
    *
    * @return float: g offset
    *
    **/
    float calcMag(int16_t mag);  // Input:	- mag = A signed 16-bit raw reading from the magnetometer.
/*****************************************************/



  private:
    bool continuousMode;        /**< Is FIFO data filled continuously */

    /**
    * @brief  readRegister(): Reads content of the IMU's registers
    *
    *   Using I2C the given register of the IMU at the provided address is read.
    *
    * @param uint8_t slaveAddress: Address of the IMU IC as given in the datasheet.
    * @param uint8_t address: Address of a given register within the IMU.
    * @return int: value of the register. -1 = read error
    *
    **/
    int readRegister(uint8_t slaveAddress, uint8_t address);


    /**
    * @brief  readRegisters(): Reads content of several registers of the IMU
    *
    *   Using I2C to read the given registers of the IMU at the start of the provided address.
    *
    * @param uint8_t slaveAddress: Address of the IMU IC as given in the datasheet.
    * @param uint8_t address: Address of a given register within the IMU.
    * @param uint8_t*data: pointer to the recieving vatiable.
    * @param size_t length: Determining the number of registers we want to read, relative to the given address.
    * @return int: value of the register. -1 = read error
    *
    **/
    int readRegisters(uint8_t slaveAddress, uint8_t address, uint8_t* data, size_t length);


    /**
    * @brief  writeRegister(): Writes content to the registers of the IMU.
    *
    *   Using I2C the given register of the IMU at the provided address is overwritten.
    *   This enables us to change settings of the IMU.
    *
    * @param uint8_t slaveAddress: Address of the IMU IC as given in the datasheet.
    * @param uint8_t address: Address of a given register within the IMU.
    * @param uint8_t value: The value to be written to the given register.
    * @return int: value of the register. 0 = write error, 1 = write success
    *
    **/
    int writeRegister(uint8_t slaveAddress, uint8_t address, uint8_t value);

  private:
    TwoWire* _wire; /**<  Instance of TwoWire for I2C communication.    */

  protected:

    float gRes, aRes, mRes;   /**<  Values for storing current resolution for the sensors. Units of these values would be DPS (or g's or Gs's) per ADC tick. This value is calculated as (sensor scale) / (2^15). */

    // _autoCalc keeps track of whether we're automatically subtracting off. accelerometer and gyroscope bias calculated in calibrate().
    // bool _autoCalc;

    /**
    * @brief  init(): Private function to setup / initiate the IMU
    *
    *   Sets up gyro, accel, and mag settings to default.
    *   To set com interface and/or addresses see begin().
    *
    * @param None.
    * @return void
    *
    **/
    void init();

    /**
    * @brief  calcgRes(): Calculate the resolution of the gyroscope.
    *
    *   This function will set the value of the gRes variable.
    *   gScale must be set prior to calling this function.
    *
    * @param None.
    * @return void
    *
    **/
	  void calcgRes();

    /**
    * @brief  calcmRes(): Calculate the resolution of the magnetometer.
    *
    *   This function will set the value of the mRes variable.
    *   mScale must be set prior to calling this function.
    *
    * @param None.
    * @return void
    *
    **/
	  void calcmRes();

    /**
    * @brief  calcaRes(): Calculate the resolution of the accelerometer.
    *
    *   This function will set the value of the aRes variable.
    *   aScale must be set prior to calling this function.
    *
    * @param None.
    * @return void
    *
    **/
	  void calcaRes();


    /**
    * @brief  initAccel(): Sets up the accelerometer to begin reading.
    *
    *   This function steps through all accelerometer related control registers.
    *   Upon exit these registers will be set as:
    *   - CTRL_REG0_XM = 0x00: FIFO disabled. HPF bypassed. Normal mode.
    *   - CTRL_REG1_XM = 0x57: 100 Hz data rate. Continuous update. all axes enabled.
    *   - CTRL_REG2_XM = 0x00:  2g scale. 773 Hz anti-alias filter BW.
    *   - CTRL_REG3_XM = 0x04: Accel data ready signal on INT1_XM pin.
    *
    * @param None.
    * @return void
    *
    **/
	void initAccel();


  /**
  * @brief  initGyro(): Sets up the gyroscope to begin reading.
  *
  *   This function steps through all five gyroscope control registers.
  *   Upon exit, the following parameters will be set:
  *   - CTRL_REG1_G = 0x0F: Normal operation mode, all axes enabled. 		95 Hz ODR, 12.5 Hz cutoff frequency.
  *   - CTRL_REG2_G = 0x00: HPF set to normal mode, cutoff frequency		set to 7.2 Hz (depends on ODR).
  *   - CTRL_REG3_G = 0x88: Interrupt enabled on INT_G (set to push-pull and active high). Data-ready output enabled on DRDY_G.
  *   - CTRL_REG4_G = 0x00: Continuous update mode. Data LSB stored in lower address. Scale set to 245 DPS. SPI mode set to 4-wire.
  *   - CTRL_REG5_G = 0x00: FIFO disabled. HPF disabled.
  *
  * @param None.
  * @return void
  *
  **/
	void initGyro();



  /**
  * @brief  initMag(): Sets up the magnetometer to begin reading.
  *
  *   This function steps through all magnetometer-related control registers.
  *   Upon exit these registers will be set as:
	*   - CTRL_REG4_XM = 0x04: Mag data ready signal on INT2_XM pin.
	*   - CTRL_REG5_XM = 0x14: 100 Hz update rate. Low resolution. Interrupt requests don't latch. Temperature sensor disabled.
	*   - CTRL_REG6_XM = 0x00:  2 Gs scale.
	*   - CTRL_REG7_XM = 0x00: Continuous conversion mode. Normal HPF mode.
	*   - INT_CTRL_REG_M = 0x09: Interrupt active-high. Enable interrupts.

  *
  * @param None.
  * @return void
  *
  **/
	void initMag();
};

extern LSM9DS1Class IMU;

//EOF
