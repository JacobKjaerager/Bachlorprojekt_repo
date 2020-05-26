/**
  *
  *
  * @file:    feature_calculation.h
  * @date:    26-03-2020 09:32:41
  * @author:  Morten Sahlertz
  *
  * @brief    Function for calculating the features of the signal
  *
  *
  *
  *
**/

#ifndef FEATURE_CALCULATION_H_
#define FEATURE_CALCULATION_H_

#include <Arduino.h>


/**
* @brief  feature_calc(): Calculate the features from the signal data from the IMU
*
*   Calculates the features from the signals data arrays, accelerometer x-axis, y-axis and z-axis
*   data arrays and gyroscopes x-axis, y-axus and z-axis data arrays. The calculated features will
*   be put in the features array.
*
*¨@param float calc_feat[]: Pointer to the array where the user wants to store the calculated feature values
* @param float acc_x_arr[]: Pointer to the array with the stored accelerometer x-axis data.
* @param float acc_y_arr[]: Pointer to the array with the stored accelerometer y-axis data.
* @param float acc_z_arr[]: Pointer to the array with the stored accelerometer z-axis data.
* @param float gyro_x_arr[]: Pointer to the array with the stored gyroscope x-axis data.
* @param float gyro_y_arr[]: Pointer to the array with the stored gyroscope y-axis data.
* @param float gyro_z_arr[]: Pointer to the array with the stored gyroscope z-axis data.
* @param char name_array[]: Pointer to the array where the new name is stored
* @param int fall_number: Int for fall number wanted in the name_array[]
* @return void:
*
**/
void feature_calc(float calc_feat[], float acc_x_sig[], float acc_y_sig[],
                  float acc_z_sig[], float gyro_x_sig[], float gyro_y_sig[],
                  float gyro_z_sig[], int signal_len);

#endif  // FEATURE_CALCULATION_H_
