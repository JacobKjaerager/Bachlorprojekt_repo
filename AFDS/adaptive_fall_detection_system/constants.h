
/**
  *
  *
  * @file:    constants.h
  * @date:    26-03-2020 09:32:41
  * @author:  Morten Sahlertz
  *
  * @brief    Constants needed for the system
  *
  *
  *
  *
**/

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

// Define the RBG LED pins
#define LEDR (22u)                    /**<  Red RBG LED on arduino nano 33 board  */
#define LEDG (23u)                    /**<  Green RBG LED on arduino nano 33 board  */
#define LEDB (24u)                    /**<  Blue RBG LED on arduino nano 33 board  */
// Define the AUDIO pin
#define AUDIO (3u)                    /**<  Pin for the audio device (digital pin number 3)  */
// Define the wake up pin for the LoPy, on digital pin 5
#define WAKE_UP (5u)                  /**<  Pin for the waking up the LoPy (digital pin number 5) */

// This constant is the chip select pin number for the SPI communaction between the
// Arduino Nano 33 BLE and the SD card reader module 
const int CS = 10;                  /**<  Chip select pin number for SPI communication between Arduino Nano 33 BLE and SD card reader  */
// This constant is the amount of features needed to calculate in the model
const int FEATURE_AMOUNT = 46;      /**<  Amounts of features calculated from the accelerometer and gyroscope data */
// This constant is the size of the name used to save the fall signal on the SD card
const int NAME_SIZE = 10;           /**<  Size of the array used to generate the name for the fall signal */
// This constant is the size of the signal, that we want to calculate features on
const int SIGNAL_SIZE = 120;        /**<  Size of the signal saved in the signal array */
// The threshold of the signal vector magnitude
const float THRESHOLD = 1.5022;        /**<  Threshold of the signal vector magnitude  */

#endif  // CONSTANTS_H_
