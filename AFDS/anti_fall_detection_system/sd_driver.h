#ifndef SD_DRIVER_H_
#define SD_DRIVER_H_

#include <SD.h>

void writeSignalValues(int cs, char name[], float acc_x_arr[], float acc_y_arr[],
                      float acc_z_arr[], float gyro_x_arr[], float gyro_y_arr[],
                      float gyro_z_arr[], int signal_length, bool overwrite);

size_t readField(File* file, char* str, size_t size, char* delim);

void readSignalValues(int cs, char name[], float acc_x_arr[], float acc_y_arr[],
                      float acc_z_arr[], float gyro_x_arr[], float gyro_y_arr[],
                      float gyro_z_arr[]);

#endif  // SD_DRIVER_H_
