


#include <Arduino.h>
#include <./GPS_Driver/TinyGPS++.h>
#include <./GPS_Driver/GPSHelper.h>



void displayInfo();

// int GPSBaud = 9600;

// Create a TinyGPS++ object
// TinyGPSPlus gps;


void setup()
{
  // Starting the Arduino hardware serial port - for readout purposes
  Serial.begin(9600);


  //  Initializing GPS Driver - this just starts the second hardware serial port.
  InitGPS();
}

void loop(){
  //  This is just for testing, and will do a readout of the GPS data to the Serial port
  PrintGPSData();

  //  Use this to read GPS data to a parcel containing readout og all the needed stuff.
  // if (isDataReady()) { GPSPackage parcel = ReadGPSData(); }


}
















//EOF
