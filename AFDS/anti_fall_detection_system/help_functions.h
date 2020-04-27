#ifndef HELP_FUNCTIONS_H_
#define HELP_FUNCTIONS_H_

#include <Arduino.h>

void push_array(float new_value, float chosen_array[], int array_length);

void generate_fall_name(char name_array[], int fall_number);

void feature_calculation(float calc_feat[], float acc_x_sig[], float acc_y_sig[],
                         float acc_z_sig[], float gyro_x_sig[], float gyro_y_sig[],
                         float gyro_z_sig[], int signal_len);

#endif  // HELP_FUNCTIONS_H_
