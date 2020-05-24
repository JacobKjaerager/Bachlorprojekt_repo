
/**
  *
  *
  * @file:    datalogger.h
  * @date:    26-03-2020 09:32:46
  * @author:  Morten Sahlertz
  *
  * @brief    Driver for reading and writing to the MicroSD card
  *
  *
  *
  *
**/

#ifndef DATALOGGER_H_
#define DATALOGGER_H_

#include <SD.h>

class datalogger{
  public:

    /**
      * @brief  writeSignalValues(): Write the signal values stored in the arrays to the SD card
      *
      *   This function generates a file on the SD card opens it and writes the array value one line at a time in a csv style.
    
      *
      * @param int cs: The int value for the chip select pin
      * @param char name[]: Pointer to the name array. This will be the name of the file on the MicroSD card.
      * @param float acc_x_arr[]: Pointer to the array with the stored accelerometer x-axis data.
      * @param float acc_y_arr[]: Pointer to the array with the stored accelerometer y-axis data.
      * @param float acc_z_arr[]: Pointer to the array with the stored accelerometer z-axis data.
      * @param float gyro_x_arr[]: Pointer to the array with the stored gyroscope x-axis data.
      * @param float gyro_y_arr[]: Pointer to the array with the stored gyroscope y-axis data.
      * @param float gyro_z_arr[]: Pointer to the array with the stored gyroscope z-axis data.
      * @param int signal_length: The length of all the signal data arrays.
      * @param bool overwrite: Bool to check whether or not to overwrite existing data.
      * @return void
      *
      **/  
    virtual void writeSignalValues(int cs, char name[], float acc_x_arr[], float acc_y_arr[],
                                   float acc_z_arr[], float gyro_x_arr[], float gyro_y_arr[],
                                   float gyro_z_arr[], int signal_length, bool overwrite);

    size_t readField(File* file, char* str, size_t size, char* delim);

    /**
    * @brief  readSignalValues(): Read the signal values stored in the file on the SD card
    *
    *   This function looks for a file on the SD card opens it and reads values one line at a time into the chosen arrays.
  
    *
    * @param int cs: The int value for the chip select pin
    * @param char name[]: Pointer to the name array. This has to be the name of the file the user wants to read on the MicroSD card.
    * @param float acc_x_arr[]: Pointer to the array where the user wants to store accelerometer x-axis data.
    * @param float acc_y_arr[]: Pointer to the array where the user wants to store accelerometer y-axis data.
    * @param float acc_z_arr[]: Pointer to the array where the user wants to store accelerometer z-axis data.
    * @param float gyro_x_arr[]: Pointer to the array where the user wants to store gyroscope x-axis data.
    * @param float gyro_y_arr[]: Pointer to the array where the user wants to store gyroscope y-axis data.
    * @param float gyro_z_arr[]: Pointer to the array where the user wants to store gyroscope z-axis data.
    * @return void
    *
    **/ 
    virtual void readSignalValues(int cs, char name[], float acc_x_arr[], float acc_y_arr[],
                                  float acc_z_arr[], float gyro_x_arr[], float gyro_y_arr[],
                                  float gyro_z_arr[]);

};

#endif  // DATALOGGER_H_
