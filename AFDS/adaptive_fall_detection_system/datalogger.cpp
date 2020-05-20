
/**
  *
  *
  * @file:    datalogger.cpp
  * @date:    26-03-2020 09:32:46
  * @author:  Morten Sahlertz
  *
  * @brief    Driver for reading and writing to the MicroSD card
  *
  *
  *
  *
**/

#include "datalogger.h"

#include <SD.h>

void writeSignalValues(int cs, char name[], float acc_x_arr[], float acc_y_arr[],
                      float acc_z_arr[], float gyro_x_arr[], float gyro_y_arr[],
                      float gyro_z_arr[], int signal_length, bool overwrite) {

  File file;
  pinMode(cs, OUTPUT);
  String dataString = "";
  bool have_informed = false;
  
  if(overwrite == true) {
    if(SD.exists(name)) {
      SD.remove(name);
    }
  }
  file = SD.open(name, FILE_WRITE);
  for (int i = 0; i < signal_length; i++) {
    dataString = String(acc_x_arr[i]) + "," + String(acc_y_arr[i]) + "," +
                 String(acc_z_arr[i]) + "," + String(gyro_x_arr[i]) + "," +
                 String(gyro_y_arr[i]) + "," + String(gyro_z_arr[i]);
    if (file) {
      file.println(dataString);
    }
    else {
      // This is for only printing once that there was an error writing to the file
      if(have_informed == false) {
        Serial.println("Error writing to file!");
        have_informed = true;
      }
    }
  } 
  file.close();
}  

size_t readField(File* file, char* str, size_t size, char* delim) {
  char ch;
  size_t n = 0;
  while ((n + 1) < size && file->read(&ch, 1) == 1) {
    // Delete CR.
    if (ch == '\r') {
      continue;
    }
    str[n++] = ch;
    if (strchr(delim, ch)) {
        break;
    }
  }
  str[n] = '\0';
  return n;
}

void readSignalValues(int cs, char name[], float acc_x_arr[], float acc_y_arr[],
                      float acc_z_arr[], float gyro_x_arr[], float gyro_y_arr[],
                      float gyro_z_arr[]) {

  File file;
  pinMode(cs, OUTPUT);

  int line = 0;
  int column = 0;

  // for testing!
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(cs)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  file = SD.open(name, FILE_READ);
  if (!file) {
    Serial.println("open error");
    return;
  }

  file.seek(0);

  size_t n;      // Length of returned field with delimiter.
  char str[20];  // Must hold longest field with delimiter and zero byte.

  // Read the file and print fields.
  while (true) {
    n = readField(&file, str, sizeof(str), ",\n");
    // done if Error or at EOF.
    if (n == 0) break;

    // Check the type of delimiter.
    if (str[n-line] == ',') {
      //Serial.print(str[n-1] == ',' ? F("comma: ") : F("endl:  "));

      // Remove the delimiter.
      str[n-line] = 0;
      // Increment column count
      column++;

    } else if (str[n-line] == '\n') {
      // Remove the delimiter.
      str[n-line] = 0;
      // Reset column count
      column = 0;
    } else {
      // At eof, too long, or read error.  Too long is error.
      // This space can be used for debugging
    }
    // Check for which column: 
    // 1 = Acc X-axis, 2 = Acc Y-axis, 3 = Acc Z-axis
    // 4 = Gyro X-axis, 5 = Gyro Y-axis, 0(6) = Gyro Z-axis
    if(column == 1){
      acc_x_arr[line] = strtod(str, nullptr);
    }
    if(column == 2){
      acc_y_arr[line] = strtod(str, nullptr);
    }
    if(column == 3){
      acc_z_arr[line] = strtod(str, nullptr);
    }
    if(column == 4){
      gyro_x_arr[line] = strtod(str, nullptr);
    }
    if(column == 5){
      gyro_y_arr[line] = strtod(str, nullptr);
    }
    if(column == 0){
      gyro_z_arr[line] = strtod(str, nullptr);
      line++;
    }

  }
  file.close();
}
