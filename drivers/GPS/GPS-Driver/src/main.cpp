//
//
//
// #include <Arduino.h>
// #include <TinyGPS++.h>


















#include <Arduino.h>
#include <TinyGPS++.h>



void displayInfo();

int GPSBaud = 9600;

// Create a TinyGPS++ object
TinyGPSPlus gps;


void setup()
{
  // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);

  // Start the software serial port at the GPS's default baud
  Serial1.begin(GPSBaud);
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (Serial1.available() > 0)
    if (gps.encode(Serial1.read()))
      displayInfo();

  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    while(true);
  }
}

void displayInfo()
{
  if (gps.location.isValid())
  {
    Serial.print("# Sattelites in view: ");
    Serial.println(gps.satellites.value() );
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
    Serial.print("Altitude: ");
  }
  else
  {
    Serial.println("Location: Not Available");
  }

  Serial.print("Date: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.println(gps.date.year());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.print("Time: ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.println(gps.time.centisecond());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.println();
  Serial.println();
  delay(1000);
}














/****************************************************************************/
//
// //  Denmark is at UTC+1 - and if not in daylight savings time we're at UTC+2
// #define UTC_COPENHAGEN  1
// #define SUMMERTIME      1
//
// //  need to somehow read date from $GPRMC and determine if we're in daylight savings time
//
// //  checksum checking
//
//
// void setup() {
//   Serial.begin(9600);
//   Serial1.begin(9600);
// }
//
// void loop() {
//   if (Serial.available()) {      // If anything comes in Serial (USB),
//     Serial1.write(Serial.read());   // read it and send it out Serial1 (pins 0 & 1)
//   }
//
//   if (Serial1.available()) {     // If anything comes in Serial1 (pins 0 & 1)
//     Serial.write(Serial1.read());   // read it and send it out Serial (USB)
//   }
// }
