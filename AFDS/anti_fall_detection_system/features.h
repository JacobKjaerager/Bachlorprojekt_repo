#ifndef FEATURES_H_
#define FEATURES_H_

#include <Arduino.h>

void feature_calculation(float calc_feat[], float acc_x_sig[], float acc_y_sig[],
                         float acc_z_sig[], float gyro_x_sig[], float gyro_y_sig[],
                         float gyro_z_sig[], int signal_len);

#endif  // FEATURES_H_
