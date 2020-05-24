
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

void datalogger::writeSignalValues(int cs, char name[], float acc_x_arr[], float acc_y_arr[],
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
};  

size_t datalogger::readField(File* file, char* str, size_t size, char* delim) {
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

void datalogger::readSignalValues(int cs, char name[], float acc_x_arr[], float acc_y_arr[],
                      float acc_z_arr[], float gyro_x_arr[], float gyro_y_arr[],
                      float gyro_z_arr[]) {

  File file;
  pinMode(cs, OUTPUT);

  int l = 0;
  int line = 0;

  // for testing!
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(cs)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  delay(3000);

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

    // Print the type of delimiter.
    if (str[n-1] == ',') {
      //Serial.print(str[n-1] == ',' ? F("comma: ") : F("endl:  "));

      // Remove the delimiter.
      str[n-1] = 0;
      // Increment line count
      line++;

    } else if (str[n-1] == '\n') {
      //Serial.print(str[n-1] == ',' ? F("comma: ") : F("endl:  "));

      // Remove the delimiter.
      str[n-1] = 0;
      // Reset line count
      line = 0;
    } else {
      // At eof, too long, or read error.  Too long is error.
      //Serial.print(file.available() ? F("error: ") : F("eof:   "));
    }
    // Print the field.
    //Serial.println(str);
    if(line == 1){
      acc_x_arr[l] = strtod(str, nullptr);
    }
    if(line == 2){
      acc_y_arr[l] = strtod(str, nullptr);
    }
    if(line == 3){
      acc_z_arr[l] = strtod(str, nullptr);
    }
    if(line == 4){
      gyro_x_arr[l] = strtod(str, nullptr);
    }
    if(line == 5){
      gyro_y_arr[l] = strtod(str, nullptr);
    }
    if(line == 0){
      gyro_z_arr[l] = strtod(str, nullptr);
      l++;
    }

  }
  file.close();
}
