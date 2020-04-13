





#include "SetupIMU.h"
#include <Math.h>

#define PRINT_CALCULATED
// #define PRINT_RAW
#define PRINT_SPEED 250
#define DECLINATION -3.45 // Declination (degrees) in Aarhus

static unsigned long lastPrint = 0;
float freeFallThreshold = 0.5;
float totalAcceleration;

bool IsInFreeFall(){
  int16_t ax_, ay_, az_;

  if ( IMU.gyroscopeAvailable() ) {
    IMU.readGyro();
  }

  ax_ = IMU.calcAccel(IMU.ax);
  ay_ = IMU.calcAccel(IMU.ay);
  az_ = IMU.calcAccel(IMU.az);

  //  use this to check maybe?
  totalAcceleration = sqrt(ax_ * ax_ + ay_ * ay_ + az_ * az_);

  //  Returning true if total magnitude of accelleration in all axes are more than or equal to the given threshold -
  //  given by the formula: √(ax_^2 + ay_^2 + az_^2)
  return (sqrt(ax_ * ax_ + ay_ * ay_ + az_ * az_) <= freeFallThreshold ? true : false );
}





void SetupACC(bool enableAcc, uint8_t scale, uint8_t sampleRate, uint8_t bandwidth ){
  IMU.settings.accel.enabled = enableAcc;
  IMU.settings.accel.enableX = enableAcc;
  IMU.settings.accel.enableY = enableAcc;
  IMU.settings.accel.enableZ = enableAcc;
  IMU.settings.accel.scale = scale;
  IMU.settings.accel.sampleRate = sampleRate;
  IMU.settings.accel.bandwidth = bandwidth;
  IMU.settings.accel.highResEnable = false;
  IMU.settings.accel.highResBandwidth = 0;
}

void SetupGyro(bool enableGyr, int scale, uint8_t sampleRate, uint8_t bandwidth, bool lowPowerMode){
  IMU.settings.gyro.enabled = enableGyr;
  IMU.settings.gyro.scale = scale;
  IMU.settings.gyro.sampleRate = sampleRate;
  IMU.settings.gyro.bandwidth = bandwidth;
  IMU.settings.gyro.lowPowerEnable = lowPowerMode;
  IMU.settings.gyro.HPFEnable = true;
  IMU.settings.gyro.HPFCutoff = 1;
  IMU.settings.gyro.flipX = false;
  IMU.settings.gyro.flipY = false;
  IMU.settings.gyro.flipZ = false;
}

void SetupMag(bool enableMag, uint8_t scale, uint8_t sampleRate, bool TempCompensateEnable, bool lowPowerMode){
  IMU.settings.mag.enabled = enableMag;
  IMU.settings.mag.scale = scale;
  IMU.settings.mag.sampleRate = sampleRate;
  IMU.settings.mag.tempCompensationEnable = TempCompensateEnable;
  IMU.settings.mag.XYPerformance = 3;
  IMU.settings.mag.ZPerformance = 3;
  IMU.settings.mag.lowPowerEnable = lowPowerMode;
  IMU.settings.mag.operatingMode = 0;
}

void SetupIMUDefaults() {
  SetupACC(true,2,1,-1);
  SetupGyro(true,2000,3,-1,false);
  SetupMag(true,1,8,true,false);
}





void StartIMU() {
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");

  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");

  Serial.print("Magnetometer sample rate = ");
  Serial.print(IMU.magneticFieldSampleRate());
  Serial.println(" Hz");

  Serial.println();
  Serial.println("Acceleration in G's");
  Serial.println("X\tY\tZ");
}

void PrintTest() {
  // Update the sensor values whenever new data is available
  if ( IMU.gyroscopeAvailable() ){
    // To read from the gyroscope,  first call the
    // readGyro() function. When it exits, it'll update the
    // gx, gy, and gz variables with the most current data.
    IMU.readGyro();
  }

  if ( IMU.accelerationAvailable() ){
    // To read from the accelerometer, first call the
    // readAccel() function. When it exits, it'll update the
    // ax, ay, and az variables with the most current data.
    IMU.readAccel();
  }

  if ( IMU.magneticFieldAvailable() ){
    // To read from the magnetometer, first call the
    // readMag() function. When it exits, it'll update the
    // mx, my, and mz variables with the most current data.
    IMU.readMag();
  }

  // To read from the magnetometer, first call the
  // readMag() function. When it exits, it'll update the
  // mx, my, and mz variables with the most current data.
  IMU.readTemp();

  if ((lastPrint + PRINT_SPEED) < millis()){
    printGyro();  // Print "G: gx, gy, gz"
    printAccel(); // Print "A: ax, ay, az"
    printMag();   // Print "M: mx, my, mz"


    // Print the heading and orientation for fun!
    // Call print attitude. The LSM9DS1's mag x and y
    // axes are opposite to the accelerometer, so my, mx are
    // substituted for each other.
    printAttitude(IMU.ax, IMU.ay, IMU.az,
                  -IMU.my, -IMU.mx, IMU.mz);
    Serial.println();
    lastPrint = millis();
  }
}


void printTemp(){
  Serial.print("Temp: ");
  Serial.println(IMU.temperature);
}

void printGyro(){
  Serial.print("G: ");
#ifdef PRINT_CALCULATED
  Serial.print(IMU.calcGyro(IMU.gx), 2);
  Serial.print(", ");
  Serial.print(IMU.calcGyro(IMU.gy), 2);
  Serial.print(", ");
  Serial.print(IMU.calcGyro(IMU.gz), 2);
  Serial.println(" deg/s");
#elif defined PRINT_RAW
  Serial.print(IMU.gx);
  Serial.print(", ");
  Serial.print(IMU.gy);
  Serial.print(", ");
  Serial.println(IMU.gz);
#endif
}

void printAccel(){
  Serial.print("A: ");
#ifdef PRINT_CALCULATED
  Serial.print(IMU.calcAccel(IMU.ax), 2);
  Serial.print(", ");
  Serial.print(IMU.calcAccel(IMU.ay), 2);
  Serial.print(", ");
  Serial.print(IMU.calcAccel(IMU.az), 2);
  Serial.println(" g");
#elif defined PRINT_RAW
  Serial.print(IMU.ax);
  Serial.print(", ");
  Serial.print(IMU.ay);
  Serial.print(", ");
  Serial.println(IMU.az);
#endif

}

void printMag(){
  Serial.print("M: ");
#ifdef PRINT_CALCULATED
  Serial.print(IMU.calcMag(IMU.mx), 2);
  Serial.print(", ");
  Serial.print(IMU.calcMag(IMU.my), 2);
  Serial.print(", ");
  Serial.print(IMU.calcMag(IMU.mz), 2);
  Serial.println(" gauss");
#elif defined PRINT_RAW
  Serial.print(IMU.mx);
  Serial.print(", ");
  Serial.print(IMU.my);
  Serial.print(", ");
  Serial.println(IMU.mz);
#endif
}

// Calculate pitch, roll, and heading.
// Pitch/roll calculations take from this app note:
// http://cache.freescale.com/files/sensors/doc/app_note/AN3461.pdf?fpsp=1
// Heading calculations taken from this app note:
// http://www51.honeywell.com/aero/common/documents/myaerospacecatalog-documents/Defense_Brochures-documents/Magnetic__Literature_Application_notes-documents/AN203_Compass_Heading_Using_Magnetometers.pdf
void printAttitude(float ax, float ay, float az, float mx, float my, float mz){
  float roll = atan2(ay, az);
  float pitch = atan2(-ax, sqrt(ay * ay + az * az));

  float heading;
  if (my == 0)
    heading = (mx < 0) ? PI : 0;
  else
    heading = atan2(mx, my);

  heading -= DECLINATION * PI / 180;

  if (heading > PI) heading -= (2 * PI);
  else if (heading < -PI) heading += (2 * PI);

  // Convert everything from radians to degrees:
  heading *= 180.0 / PI;
  pitch *= 180.0 / PI;
  roll  *= 180.0 / PI;

  Serial.print("Pitch, Roll: ");
  Serial.print(pitch, 2);
  Serial.print(", ");
  Serial.println(roll, 2);
  Serial.print("Heading: "); Serial.println(heading, 2);
}
