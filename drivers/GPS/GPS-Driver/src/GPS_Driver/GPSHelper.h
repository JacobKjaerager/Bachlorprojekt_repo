

/*! \brief This is a helper "class" to make the usage of the TinyGPS++ library.
 *
 *			Description here
 *
 *
 *
 *	Filename:	GPSHelper.h
 *	Path:		./GPS_Driver/
 *	Created:	12-04-2020 10:56:48
 *	Author:		Tonni Lutze
 *
 *	Major change #1:
 */


#include "TinyGPS++.h"



/**
 * \brief Struct containing all the data needed from GPS
 *
 * \param None.
 *
 * \return n/a
 */
struct GPSPackage{
  double    Latitude;     // 4 byte
  double    Longitude;    // 4 byte
  double    Altitude;     // 4 byte
  uint16_t  Elevation;    // 2 byte
  uint16_t  Azimuth;      // 2 byte
  uint8_t   Sattelites;   // 1 byte - There are 24 sattelites in orbit, of those a theoretical max of 14 is ever visible at any given time, thus we only need one byte.
  uint16_t  DateYear;     // 2 Byte
  uint8_t   DateMonth;    // 1 Byte
  uint8_t   DateDay;      // 1 Byte
  uint8_t   TimeHour;     // 1 Byte
  uint8_t   TimeMinute;   // 1 Byte
  uint8_t   TimeSecond;   // 1 Byte
};


/**
 * \brief Initializing GPS Driver
 *        This really just starts the second hardware serial port.
 *        //TODO: enable/disable pin on nano to power on/off the GPS.
 *
 * \param None.
 *
 * \return void
 */
void InitGPS();

/**
 * \brief Function to do a Readout from the GPS.
 *        Do a check with isDataReady() to make sure data is ready before using.
 *
 * \param None.
 *
 * \return GPSPackage
 */
GPSPackage ReadGPSData();

/**
 * \brief Function to do a printout to Serial Port of the collected GPS data.
 *        Mainly for testing purposes.
 *
 * \param None.
 *
 * \return void
 */
void PrintGPSData();

/**
 * \brief Very simple function to check if data is ready from GPS.
 *
 * \param None.
 *
 * \return Bool
 */
bool isDataReady();























//  EOF
