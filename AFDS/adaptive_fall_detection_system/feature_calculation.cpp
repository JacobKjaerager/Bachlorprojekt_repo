/**
  *
  *
  * @file:    feature_calculation.cpp
  * @date:    26-03-2020 09:32:41
  * @author:  Morten Sahlertz
  *
  * @brief    Function for calculating the features of the signal
  *
  *
  *
  *
**/

#include "feature_calculation.h"

#include <Arduino.h>

void feature_calc(float calc_feat[], float acc_x_sig[], float acc_y_sig[],
                         float acc_z_sig[], float gyro_x_sig[], float gyro_y_sig[],
                         float gyro_z_sig[], int signal_len) {
  // Instanciate x-axis variables with first signal value
  float max_x_acc = acc_x_sig[0], min_x_acc = acc_x_sig[0], tot_x_acc = acc_x_sig[0];
  float max_slope_x_acc = 0, sum_acc_x = 0;
  float max_x_peaks[3] = {0}, min_x_peaks[3] = {0};
  max_x_peaks[0] = acc_x_sig[0], min_x_peaks[0] = acc_x_sig[0];
  // Instanciate y-axis variables with first signal value
  float max_y_acc = acc_y_sig[0], min_y_acc = acc_y_sig[0], tot_y_acc = acc_y_sig[0];
  float max_slope_y_acc = 0, sum_acc_y = 0;
  float max_y_peaks[3] = {0}, min_y_peaks[3] = {0};
  max_y_peaks[0] = acc_y_sig[0], min_y_peaks[0] = acc_y_sig[0];
  min_y_peaks[1] = acc_y_sig[0], min_y_peaks[2] = acc_y_sig[0];
  // Instanciate z-axis variables with first signal value
  float max_z_acc = acc_z_sig[0], min_z_acc = acc_z_sig[0], tot_z_acc = acc_z_sig[0];
  float max_slope_z_acc = 0, sum_acc_z = 0;
  float max_z_peaks[3] = {0}, min_z_peaks[3] = {0};
  max_z_peaks[0] = acc_z_sig[0], min_z_peaks[0] = acc_z_sig[0];
  // Instanciate x-axis variables with first signal value
  float max_x_gyro = gyro_x_sig[0], min_x_gyro = gyro_x_sig[0], tot_x_gyro = gyro_x_sig[0];
  float max_slope_x_gyro = 0, sum_gyro_x = 0;
  // Instanciate y-axis variables with first signal value
  float max_y_gyro = gyro_y_sig[0], min_y_gyro = gyro_y_sig[0], tot_y_gyro = gyro_y_sig[0];
  float max_slope_y_gyro = 0, sum_gyro_y = 0;
  // Instanciate z-axis variables with first signal value
  float max_z_gyro = gyro_z_sig[0], min_z_gyro = gyro_z_sig[0], tot_z_gyro = gyro_z_sig[0];
  float max_slope_z_gyro = 0, sum_gyro_z = 0;
  // Instanciate magnitude calculation variables
  float accel_mag = sqrt(sq(acc_x_sig[0])+sq(acc_y_sig[0])+sq(acc_z_sig[0]));
  float max_accel_mag = accel_mag, min_accel_mag = accel_mag;
  // Instanciate windowed standard deviation magnitude variables
  float mean_std_x = 0, mean_std_y = 0, mean_std_z = 0;
  float sum_std_x = 0, sum_std_y = 0, sum_std_z = 0;
  float std = 0, std_hoz = 0, max_std = 0, max_std_hoz = 0;
  float window_len = 20;
  // Run through signal making calculations
  for (int index = 1; index < signal_len; index++) {
    // Calculate acc X-axis features
    max_x_acc = max(acc_x_sig[index], max_x_acc);
    min_x_acc = min(acc_x_sig[index], min_x_acc);
    max_slope_x_acc = max(abs(acc_x_sig[index-1]-acc_x_sig[index]), max_slope_x_acc);
    tot_x_acc += acc_x_sig[index];
    // Calculate acc Y-axis features
    max_y_acc = max(acc_y_sig[index], max_y_acc);
    min_y_acc = min(acc_y_sig[index], min_y_acc);
    max_slope_y_acc = max(abs(acc_y_sig[index-1]-acc_y_sig[index]), max_slope_y_acc);
    tot_y_acc += acc_y_sig[index];
    // Calculate acc Z-axis features
    max_z_acc = max(acc_z_sig[index], max_z_acc);
    min_z_acc = min(acc_z_sig[index], min_z_acc);
    max_slope_z_acc = max(abs(acc_z_sig[index-1]-acc_z_sig[index]), max_slope_z_acc);
    tot_z_acc += acc_z_sig[index];
    // Calculate gyro X-axis features
    max_x_gyro = max(gyro_x_sig[index], max_x_gyro);
    min_x_gyro = min(gyro_x_sig[index], min_x_gyro);
    max_slope_x_gyro = max(abs(gyro_x_sig[index-1]-gyro_x_sig[index]), max_slope_x_gyro);
    tot_x_gyro += gyro_x_sig[index];
    // Calculate gyro Y-axis features
    max_y_gyro = max(gyro_y_sig[index], max_y_gyro);
    min_y_gyro = min(gyro_y_sig[index], min_y_gyro);
    max_slope_y_gyro = max(abs(gyro_y_sig[index-1]-gyro_y_sig[index]), max_slope_y_gyro);
    tot_y_gyro += gyro_y_sig[index];
    // Calculate gyro Z-axis features
    max_z_gyro = max(gyro_z_sig[index], max_z_gyro);
    min_z_gyro = min(gyro_z_sig[index], min_z_gyro);
    max_slope_z_gyro = max(abs(gyro_z_sig[index-1]-gyro_z_sig[index]), max_slope_z_gyro);
    tot_z_gyro += gyro_z_sig[index];
    // Calculate acceleration magnitude
    accel_mag = sqrt(sq(acc_x_sig[index])+sq(acc_y_sig[index])+sq(acc_z_sig[index]));
    max_accel_mag = max(accel_mag, max_accel_mag);
    min_accel_mag = min(accel_mag, min_accel_mag);
    // Calculate average of max and min peaks
    if (index < signal_len-1) { // This is to make sure, the loop doesn't reach end of signal
      // Calculate the x max peaks
      if (acc_x_sig[index-1] < acc_x_sig[index] && acc_x_sig[index] >= acc_x_sig[index+1]) {
        if(index < signal_len-2 && acc_x_sig[index] == acc_x_sig[index+1] && acc_x_sig[index+2] > acc_x_sig[index+1]) {
           // This is for checking for the definition of a peak, if it evens out and the next sample is bigger, it is not defined as a peak
           // So first there is a check to see if the function is at the end of the signal, and since it is evaluated first, incase it is
           // at the end of the signal the function won't reach beyond the 
        }
        else {
          if (max_x_peaks[0] <= acc_x_sig[index]) {
            max_x_peaks[2] = max_x_peaks[1];
            max_x_peaks[1] = max_x_peaks[0];
            max_x_peaks[0] = acc_x_sig[index];
          }
          else if (max_x_peaks[1] <= acc_x_sig[index] ) {
            max_x_peaks[2] = max_x_peaks[1];
            max_x_peaks[1] = acc_x_sig[index];
          }
          else if (max_x_peaks[2] <= acc_x_sig[index] ) {
            max_x_peaks[2] = acc_x_sig[index];
          }
        }
      }
      // Calculate the x min peaks
      else if (acc_x_sig[index-1] > acc_x_sig[index] && acc_x_sig[index] <= acc_x_sig[index+1]) {
        if(index < signal_len-2 && acc_x_sig[index] == acc_x_sig[index+1] && acc_x_sig[index+2] < acc_x_sig[index+1]) {
        // This is for checking for the definition of a peak, if it evens out and the next sample is smaller, it is not defined as a peak
        // So first there is a check to see if the function is at the end of the signal, and since it is evaluated first, incase it is
        // at the end of the signal the function won't reach beyond the 
        }
        else {
          if (min_x_peaks[0] >= acc_x_sig[index]) {
            min_x_peaks[2] = min_x_peaks[1];
            min_x_peaks[1] = min_x_peaks[0];
            min_x_peaks[0] = acc_x_sig[index];
          }
          else if (min_x_peaks[1] >= acc_x_sig[index] ) {
            min_x_peaks[2] = min_x_peaks[1];
            min_x_peaks[1] = acc_x_sig[index];
          }
          else if (min_x_peaks[2] >= acc_x_sig[index] ) {
            min_x_peaks[2] = acc_x_sig[index];
          }
        }
      }
      // Calculate the y max peaks
      if (acc_y_sig[index-1] < acc_y_sig[index] && acc_y_sig[index] >= acc_y_sig[index+1]) {
        if(index < signal_len-2 && acc_y_sig[index] == acc_y_sig[index+1] && acc_y_sig[index+2] > acc_y_sig[index+1]) {
           // This is for checking for the definition of a peak, if it evens out and the next sample is bigger, it is not defined as a peak
           // So first there is a check to see if the function is at the end of the signal, and since it is evaluated first, incase it is
           // at the end of the signal the function won't reach beyond the 
        }
        else {
          if (max_y_peaks[0] <= acc_y_sig[index]) {
            max_y_peaks[2] = max_y_peaks[1];
            max_y_peaks[1] = max_y_peaks[0];
            max_y_peaks[0] = acc_y_sig[index];
          }
          else if (max_y_peaks[1] <= acc_y_sig[index] ) {
            max_y_peaks[2] = max_y_peaks[1];
            max_y_peaks[1] = acc_y_sig[index];
          }
          else if (max_y_peaks[2] <= acc_y_sig[index] ) {
            max_y_peaks[2] = acc_y_sig[index];
          }
        }
      }
      // Calculate the y min peaks
      else if (acc_y_sig[index-1] > acc_y_sig[index] && acc_y_sig[index] <= acc_y_sig[index+1]) {
        if(index < signal_len-2 && acc_y_sig[index] == acc_y_sig[index+1] && acc_y_sig[index+2] < acc_y_sig[index+1]) {
        // This is for checking for the definition of a peak, if it evens out and the next sample is smaller, it is not defined as a peak
        // So first there is a check to see if the function is at the end of the signal, and since it is evaluated first, incase it is
        // at the end of the signal the function won't reach beyond the 
        }
        else {
          if (min_y_peaks[0] >= acc_y_sig[index]) {
            min_y_peaks[2] = min_y_peaks[1];
            min_y_peaks[1] = min_y_peaks[0];
            min_y_peaks[0] = acc_y_sig[index];
          }
          else if (min_y_peaks[1] >= acc_y_sig[index] ) {
            min_y_peaks[2] = min_y_peaks[1];
            min_y_peaks[1] = acc_y_sig[index];
          }
          else if (min_y_peaks[2] >= acc_y_sig[index] ) {
            min_y_peaks[2] = acc_y_sig[index];
          }
        }
      }
      // Calculate the z max peaks
      if (acc_z_sig[index-1] < acc_z_sig[index] && acc_z_sig[index] >= acc_z_sig[index+1]) {
        if(index < signal_len-2 && acc_z_sig[index] == acc_z_sig[index+1] && acc_z_sig[index+2] > acc_z_sig[index+1]) {
        // This is for checking for the definition of a peak, if it evens out and the next sample is bigger, it is not defined as a peak
        // So first there is a check to see if the function is at the end of the signal, and since it is evaluated first, incase it is
        // at the end of the signal the function won't reach beyond the 
        }
        else {
          if (max_z_peaks[0] <= acc_z_sig[index]) {
            max_z_peaks[2] = max_z_peaks[1];
            max_z_peaks[1] = max_z_peaks[0];
            max_z_peaks[0] = acc_z_sig[index];
          }
          else if (max_z_peaks[1] <= acc_z_sig[index] ) {
            max_z_peaks[2] = max_z_peaks[1];
            max_z_peaks[1] = acc_z_sig[index];
          }
          else if (max_z_peaks[2] <= acc_z_sig[index] ) {
            max_z_peaks[2] = acc_z_sig[index];
          }
        }
      }
      // Calculate the z min peaks
      else if (acc_z_sig[index-1] > acc_z_sig[index] && acc_z_sig[index] <= acc_z_sig[index+1]) {
        if(index < signal_len-2 && acc_z_sig[index] == acc_z_sig[index+1] && acc_z_sig[index+2] < acc_z_sig[index+1]) {
        // This is for checking for the definition of a peak, if it evens out and the next sample is smaller, it is not defined as a peak
        // So first there is a check to see if the function is at the end of the signal, and since it is evaluated first, incase it is
        // at the end of the signal the function won't reach beyond the 
        }
        else {
          if (min_z_peaks[0] >= acc_z_sig[index]) {
            min_z_peaks[2] = min_z_peaks[1];
            min_z_peaks[1] = min_z_peaks[0];
            min_z_peaks[0] = acc_z_sig[index];
          }
          else if (min_z_peaks[1] >= acc_z_sig[index] ) {
            min_z_peaks[2] = min_z_peaks[1];
            min_z_peaks[1] = acc_z_sig[index];
          }
          else if (min_z_peaks[2] >= acc_z_sig[index] ) {
            min_z_peaks[2] = acc_z_sig[index];
          }
        }
      }
    }
  }
  // Calculate the means accelerometer
  float mean_acc_x = tot_x_acc / signal_len;
  float mean_acc_y = tot_y_acc / signal_len;
  float mean_acc_z = tot_z_acc / signal_len;
  // Calculate the means gyroscope
  float mean_gyro_x = tot_x_gyro / signal_len;
  float mean_gyro_y = tot_y_gyro / signal_len;
  float mean_gyro_z = tot_z_gyro / signal_len;
  // Run through signal again to calculate standard deviation and the
  // standard deviation magnitude
  for (int index = 0; index < signal_len-window_len; index++) {
    // Calculate the mean for the window
    for (int i = 0; i < window_len; i++) {
      mean_std_x += acc_x_sig[index+i];
      mean_std_y += acc_y_sig[index+i];
      mean_std_z += acc_z_sig[index+i];
    }
    // Calculate the standard deviation for the window
    for (int i = 0; i < window_len; i++) {
      sum_std_x += sq(acc_x_sig[index+i]-(mean_std_x/window_len));
      sum_std_y += sq(acc_y_sig[index+i]-(mean_std_y/window_len));
      sum_std_z += sq(acc_z_sig[index+i]-(mean_std_z/window_len));
    }
    // Calculate the standard deviation magnitudes and save the maximum
    std_hoz = sqrt(sq(sqrt(sum_std_x/window_len))+sq(sqrt(sum_std_z/window_len)));
    max_std_hoz = max(max_std_hoz, std_hoz);
    std = sqrt(sq(sqrt(sum_std_x/window_len))+sq(sqrt(sum_std_y/window_len))+sq(sqrt(sum_std_z/window_len)));
    max_std = max(max_std, std);
    // Reset the windows variables
    mean_std_x = mean_std_y = mean_std_z = 0;
    sum_std_x = sum_std_y = sum_std_z = 0;
    // Sum for standard deviation accelerometer
    sum_acc_x += sq(acc_x_sig[index]-mean_acc_x);
    sum_acc_y += sq(acc_y_sig[index]-mean_acc_y);
    sum_acc_z += sq(acc_z_sig[index]-mean_acc_z);
    // Sum for standard deviation gyroscope
    sum_gyro_x += sq(gyro_x_sig[index]-mean_gyro_x);
    sum_gyro_y += sq(gyro_y_sig[index]-mean_gyro_y);
    sum_gyro_z += sq(gyro_z_sig[index]-mean_gyro_z);
  }
  // Run through the last 20 samples to get the full sum of the signal
  for (int index = signal_len-window_len; index < signal_len; index++) {
    // Sum for standard deviation accelerometer
    sum_acc_x += sq(acc_x_sig[index]-mean_acc_x);
    sum_acc_y += sq(acc_y_sig[index]-mean_acc_y);
    sum_acc_z += sq(acc_z_sig[index]-mean_acc_z);
    // Sum for standard deviation gyroscope
    sum_gyro_x += sq(gyro_x_sig[index]-mean_gyro_x);
    sum_gyro_y += sq(gyro_y_sig[index]-mean_gyro_y);
    sum_gyro_z += sq(gyro_z_sig[index]-mean_gyro_z);
  }
  // Calculate the standard deviations accelerometer
  float std_dev_acc_x = sqrt(sum_acc_x/signal_len);
  float std_dev_acc_y = sqrt(sum_acc_y/signal_len);
  float std_dev_acc_z = sqrt(sum_acc_z/signal_len);
  // Calculate the standard deviations gyroscope
  float std_dev_gyro_x = sqrt(sum_gyro_x/signal_len);
  float std_dev_gyro_y = sqrt(sum_gyro_y/signal_len);
  float std_dev_gyro_z = sqrt(sum_gyro_z/signal_len);
  // Add the features to
  calc_feat[0] = std_dev_acc_x; // Standard deviation of signal
  calc_feat[1] = std_dev_acc_y; // Standard deviation of signal
  calc_feat[2] = std_dev_acc_z; // Standard deviation of signal
  calc_feat[3] = std_dev_gyro_x; // Standard deviation of signal
  calc_feat[4] = std_dev_gyro_y; // Standard deviation of signal
  calc_feat[5] = std_dev_gyro_z; // Standard deviation of signal
  calc_feat[6] = mean_acc_x; // Mean of signal
  calc_feat[7] = mean_acc_y; // Mean of signal
  calc_feat[8] = mean_acc_z; // Mean of signal
  calc_feat[9] = mean_gyro_x; // Mean of signal
  calc_feat[10] = mean_gyro_y; // Mean of signal
  calc_feat[11] = mean_gyro_z; // Mean of signal
  calc_feat[12] = max_x_acc; // Maximum of signal
  calc_feat[13] = max_y_acc; // Maximum of signal
  calc_feat[14] = max_z_acc; // Maximum of signal
  calc_feat[15] = max_x_gyro; // Maximum of signal
  calc_feat[16] = max_y_gyro; // Maximum of signal
  calc_feat[17] = max_z_gyro; // Maximum of signal
  calc_feat[18] = min_x_acc; // Minimum of signal
  calc_feat[19] = min_y_acc; // Minimum of signal
  calc_feat[20] = min_z_acc; // Minimum of signal
  calc_feat[21] = min_x_gyro; // Minimum of signal
  calc_feat[22] = min_y_gyro; // Minimum of signal
  calc_feat[23] = min_z_gyro; // Minimum of signal
  calc_feat[24] = max_x_acc-min_x_acc; // P2P of signal
  calc_feat[25] = max_y_acc-min_y_acc; // P2P of signal
  calc_feat[26] = max_z_acc-min_z_acc; // P2P of signal
  calc_feat[27] = max_x_gyro-min_x_gyro; // P2P of signal
  calc_feat[28] = max_y_gyro-min_y_gyro; // P2P of signal
  calc_feat[29] = max_z_gyro-min_z_gyro; // P2P of signal
  calc_feat[30] = (max_x_peaks[0]+max_x_peaks[1]+max_x_peaks[2])/3; // Avr of peaks
  calc_feat[31] = (min_x_peaks[0]+min_x_peaks[1]+min_x_peaks[2])/3; // Avr of peaks
  calc_feat[32] = (max_y_peaks[0]+max_y_peaks[1]+max_y_peaks[2])/3; // Avr of peaks
  calc_feat[33] = (min_y_peaks[0]+min_y_peaks[1]+min_y_peaks[2])/3; // Avr of peaks
  calc_feat[34] = (max_z_peaks[0]+max_z_peaks[1]+max_z_peaks[2])/3; // Avr of peaks
  calc_feat[35] = (min_z_peaks[0]+min_z_peaks[1]+min_z_peaks[2])/3; // Avr of peaks
  calc_feat[36] = max_accel_mag; // Max acceleration magnitude
  calc_feat[37] = min_accel_mag; // Min acceleration magnitude
  calc_feat[38] = max_std_hoz; // Std horizon
  calc_feat[39] = max_std; // std dev magnitude
  calc_feat[40] = max_slope_x_acc;
  calc_feat[41] = max_slope_y_acc;
  calc_feat[42] = max_slope_z_acc;
  calc_feat[43] = max_slope_x_gyro;
  calc_feat[44] = max_slope_y_gyro;
  calc_feat[45] = max_slope_z_gyro;
}
