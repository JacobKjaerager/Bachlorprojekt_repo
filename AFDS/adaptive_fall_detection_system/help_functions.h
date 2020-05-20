/**
  *
  *
  * @file:    help_functions.h
  * @date:    26-03-2020 09:32:41
  * @author:  Morten Sahlertz
  *
  * @brief    Functions for helping the functionality of the system, most importantly the feature calculation
  *
  *
  *
  *
**/

#ifndef HELP_FUNCTIONS_H_
#define HELP_FUNCTIONS_H_

#include <Arduino.h>

/**
* @brief  push_array(): Push the new value into the array
*
*   Pushes the new value into the chosen array, and pops the last array value
*   out.
*
* @param float new_value: The new value to push into the array.
* @param float chosen_array[]: Pointer to the chosen array for pushing the new value.
* @param int array_length: Int for the length of the chosen array
* @return void:
*
**/
void push_array(float new_value, float chosen_array[], int array_length);

/**
* @brief  generate_fall_name(): Generate a fall name from a fall number
*
*   Generate a new fall name with "FALL" + fall_number + ".csv", so a new fall
*   data file can be generated on the MicroSD card.
*
* @param char name_array[]: Pointer to the array where the new name is stored
* @param int fall_number: Int for fall number wanted in the name_array[]
* @return void:
*
**/
void generate_fall_name(char name_array[], int fall_number);

/**
* @brief  feature_calculation(): Calculate the features from the signal data from the IMU
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
void feature_calculation(float calc_feat[], float acc_x_sig[], float acc_y_sig[],
                         float acc_z_sig[], float gyro_x_sig[], float gyro_y_sig[],
                         float gyro_z_sig[], int signal_len);

#endif  // HELP_FUNCTIONS_H_
