#ifndef SD_READ_H_
#define SD_READ_H_

#include <SD.h>

size_t readField(File* file, char* str, size_t size, char* delim);

void readSignalValues(int cs, char name[], float acc_x_arr[], float acc_y_arr[],
                      float acc_z_arr[], float gyro_x_arr[], float gyro_y_arr[],
                      float gyro_z_arr[]);

#endif  // TENSORFLOW_LITE_EXPERIMENTAL_MICRO_EXAMPLES_HELLO_WORLD_CONSTANTS_H_
