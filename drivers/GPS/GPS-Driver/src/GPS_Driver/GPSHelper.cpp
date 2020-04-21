

/*! \brief This is a helper "class" to make the usage of the TinyGPS++ library.
 *
 *			This is a helper "class" to make the usage of the TinyGPS++ library.
 *
 *
 *
 *	Filename:	GPSHelper.cpp                                                      \n
 *	Path:		./GPS_Driver/                                                        \n
 *	Created:	12-04-2020 10:56:48                                                \n
 *	Author:		Tonni Lutze                                                         \n
 *
 *	Major change #1:
 */

#include "GPSHelper.h"

int GPSBaud = 9600;

//  Creating an instance of the GPS object
TinyGPSPlus gps;

void InitGPS(){
  // Start the secondary hardware serial port at the GPS's default baud rate
  Serial1.begin(GPSBaud);
}

//  simple function to print out the GPS dataa directly to the serial port
void displayInfo() {
  GPSPackage parcel;
  parcel = ReadGPSData();

  if (gps.location.isValid()) {
    Serial.print("# Sattelites in view: ");
    Serial.println(parcel.Sattelites);
    Serial.print("Latitude: ");
    Serial.println(parcel.Latitude, 6);
    Serial.print("Longitude: ");
    Serial.println(parcel.Longitude, 6);
    Serial.print("Altitude: ");
    Serial.println(parcel.Altitude);
    Serial.print("Elevation (deg): ");
    Serial.println(parcel.Elevation);
    Serial.print("Azimuth (deg): ");
    Serial.println(parcel.Azimuth);
  }
  else {
    Serial.println("Location: Not Available");
  }

  Serial.print("Date(dd/mm/yyyy): ");
  if (gps.date.isValid()) {
    Serial.print(parcel.DateDay);
    Serial.print("/");
    Serial.print(parcel.DateMonth);
    Serial.print("/");
    Serial.println(parcel.DateYear);
  }
  else {
    Serial.println("Not Available");
  }

  Serial.print("Time: ");
  if (gps.time.isValid()) {
    if (parcel.TimeHour < 10) Serial.print(F("0"));
    Serial.print(parcel.TimeHour);
    Serial.print(":");
    if (parcel.TimeMinute < 10) Serial.print(F("0"));
    Serial.print(parcel.TimeMinute);
    Serial.print(":");
    if (parcel.TimeSecond < 10) Serial.print(F("0"));
    Serial.print(parcel.TimeSecond);
  }
  else {
    Serial.println("Not Available");
  }

  Serial.println();
  Serial.println();
  delay(1000);
}

bool isDataReady(){
  return Serial1.available();
}

GPSPackage ReadGPSData(){
  GPSPackage parcel;
  while (Serial1.available() > 0){
    //  Reading the data from the serial port and saving it in the parcel object.
    if (gps.encode(Serial1.read())){
      parcel.Latitude   = gps.location.lat();
      parcel.Longitude  = gps.location.lng();
      parcel.Elevation  = gps.details.Elevation();
      parcel.Altitude   = gps.altitude.meters();
      parcel.Sattelites = gps.satellites.value();
      parcel.Azimuth    = gps.details.Azimuth();
      parcel.DateYear   = gps.date.year();
      parcel.DateMonth  = gps.date.month();
      parcel.DateDay    = gps.date.day();
      parcel.TimeHour   = gps.time.hour();
      parcel.TimeMinute = gps.time.minute();
      parcel.TimeSecond = gps.time.second();
      return parcel;
    }
  }
  //  Timeout functionality. If 5000 milliseconds pass and there are no characters
  //  coming from the serial port, show a "No GPS detected" error and "Halt"
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected - check wireing and restart.");
    while(true);
  }
  //  Returning empty parcel - You should not reach this point unless you forget
  //  to check if theres available data before calling this function.
  return parcel;
}


void PrintGPSData(){

  // This displays information every time a new sentence is correctly encoded.
  while (Serial1.available() > 0)
    if (gps.encode(Serial1.read()))
      displayInfo();

  //  Timeout functionality. If 5000 milliseconds pass and there are no characters
  //  coming from the serial port, show a "No GPS detected" error and "Halt"
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected - check wireing and restart.");
    while(true);
  }
}
