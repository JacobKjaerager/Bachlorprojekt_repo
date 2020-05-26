
/**
  *
  *
  *	@file:		SetupIMU.h
  *	@date:		26-03-2020 10:02:01
  *	@author:	Tonni Lutze
  *
  *	@brief    Helperclass for IMU Driver
  *
  *
  *
  *
**/


// #include "LSM9DS1_Types.h"

#include "LSM9DS1_driver.h"


/**
  * @brief  StartIMU(): Start the IMU and print the settings
  *
  *   Calls the Drivers initiation method for the IMU, which sets up the default
  *   values for the components sampletates.
  *   After initialization the settings are printed (this is mainly for testing and debugging)
  *
  * @param None.
  * @return void
  *
  **/
void StartIMU();

/**
  * @brief  PrintTest(): Readout from the IMU is printet to UART
  *
  * This function is mainly for testing and debugging
  *
  * @param None.
  * @return void
  **/
void PrintTest();

/**
  * @brief  IsInFreeFall(): checks if sensor is in a free fall
  *
  * Using a Threshold this function calculates whether the sensor is in free fall
  * and returns a boolean
  *
  * atm. Threhold is hardcoded to 0.5
  *
  * @param None.
  * @return bool
  **/
bool IsInFreeFall();

/**
  * @brief  SetupIMUDefaults(): Sets default settings for the IMU
  *
  * Using:
  * SetupACC(bool enableAcc, uint8_t scale, uint8_t sampleRate, uint8_t bandwidth )
  * SetupGyro(bool enableGyr, int scale, uint8_t sampleRate, uint8_t bandwidth, bool lowPowerMode)
  * and
  * SetupMag(bool enableMag, uint8_t scale, uint8_t sampleRate, bool TempCompensateEnable, bool lowPowerMode)
  * it sets the following settings:
  *
  * SetupACC(true,2,1,-1);
  * SetupGyro(true,2000,3,-1,false);
  * SetupMag(true,1,8,true,false);
  *
  * @param None.
  * @return void
  **/
void SetupIMUDefaults();

/**
  * @brief  SetupACC(): Setup accessor for the accelerometer part of the IMU
  *
  * The settings control the behavior of the accelerometer.
  * NOTE: If the gyroscope is enabled its settings SUPERCEDES the accelerometer settings!
  *       Specifically the samplerate.
  *
  * @param bool enableAcc: Enables or disables the accelerometer in general
  * @param uint8_t scale: Sets the full-scale range. Valid options are: 2, 4, 8, or 16
  * @param uint8_t sampleRate: Sets the output data rate (ODR), Valid options are: 1-6 corresponding to 10, 50, 119, 238, 476 or 952 Hz
  * @param uint8_t bandwidth: Sets the anti-aliasing filter bandwidth. Valid options are: -1 to 3. -1 = bandwidth determined by sample rate
  * @return void
  **/
void SetupACC(bool enableAcc, uint8_t scale, uint8_t sampleRate, uint8_t bandwidth);

/**
  * @brief  SetupGyro(): Setup accessor for the gyroscope part of the IMU
  *
  * The settings control the behavior of the gyroscope.
  * NOTE: If the gyroscope is enabled its settings SUPERCEDES the accelerometer settings!
  *       Specifically the samplerate.
  *
  * @param bool enableGyr: Enables or disables the gyroscope in general
  * @param uint8_t scale: Sets the full-scale range. Valid options are: 245, 500 or 2000
  * @param uint8_t sampleRate: Sets the output data rate (ODR), Valid options are: 1-6 corresponding to 14.9, 59.5, 119, 238, 476 or 952 Hz
  * @param uint8_t bandwidth: Sets the anti-aliasing filter bandwidth. Valid options are: -1 (not fully implemented)
  * @param bool lowPowerMode: Enables or disables low power mode.
  * @return void
  **/
void SetupGyro(bool enableGyr, int scale, uint8_t sampleRate, uint8_t bandwidth, bool lowPowerMode);

/**
  * @brief  SetupMag(): Setup accessor for the magnetometer part of the IMU
  *
  * The settings control the behavior of the magnetometer.
  *
  * @param bool enableMag: Enables or disables the magnetometer in general
  * @param uint8_t scale: Sets the full-scale range. Valid options are: 2, 4, 8 or 16
  * @param uint8_t sampleRate: Sets the output data rate (ODR), Valid options are: 1-6 corresponding to 0.625, 1.25, 2.5, 5, 10, 20, 40, 80 Hz
  * @param bool TempCompensateEnable: Enable or disable compensation for offset/drift created by temperature.
  * @param bool lowPowerMode: Enables or disables low power mode.
  * @return void
  **/
void SetupMag(bool enableMag, uint8_t scale, uint8_t sampleRate, bool TempCompensateEnable, bool lowPowerMode);
// void SetupDefaults();

/**
  * @brief  printGyro(): Gyroscope readout from the IMU is printet to UART
  *
  * This function is mainly for testing and debugging
  *
  * @param None.
  * @return void
  **/
void printGyro();

/**
  * @brief  printAccel(): Accelerometer readout from the IMU is printet to UART
  *
  * This function is mainly for testing and debugging
  *
  * @param None.
  * @return void
  **/
void printAccel();

/**
  * @brief  printMag(): Magnetometer readout from the IMU is printet to UART
  *
  * This function is mainly for testing and debugging
  *
  * @param None.
  * @return void
  **/
void printMag();

/**
  * @brief  printTemp(): Temperature readout from the IMU is printet to UART
  *
  * This function is mainly for testing and debugging
  *
  * @param None.
  * @return void
  **/
void printTemp();

/**
  * @brief  printAttitude(): Calculate and prints pitch, roll, and heading.
  *
  * This function is mainly for testing and debugging
  * Pitch/roll calculations take from this app note: http://cache.freescale.com/files/sensors/doc/app_note/AN3461.pdf?fpsp=1
  * Heading calculations taken from this app note: http://www51.honeywell.com/aero/common/documents/myaerospacecatalog-documents/Defense_Brochures-documents/Magnetic__Literature_Application_notes-documents/AN203_Compass_Heading_Using_Magnetometers.pdf
  *
  * @param float ax: Accelerometer X axis from IMU object.
  * @param float ay: Accelerometer Y axis from IMU object.
  * @param float az: Accelerometer Z axis from IMU object.
  * @param float mx: Magnetometer X axis from IMU object.
  * @param float my: Magnetometer Y axis from IMU object.
  * @param float mz: Magnetometer Z axis from IMU object.
  * @return void
  **/
void printAttitude(float ax, float ay, float az, float mx, float my, float mz);


















//EOF
