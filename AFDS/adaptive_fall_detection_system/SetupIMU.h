



// #include "LSM9DS1_Types.h"

#include "LSM9DS1_driver_TKL.h"



void StartIMU();
void PrintTest();

bool IsInFreeFall();

void SetupIMUDefaults();

void SetupACC(bool enableAcc, uint8_t scale, uint8_t sampleRate, uint8_t bandwidth);

void SetupGyro(bool enableGyr, int scale, uint8_t sampleRate, uint8_t bandwidth, bool lowPowerMode);

void SetupMag(bool enableMag, uint8_t scale, uint8_t sampleRate, bool TempCompensateEnable, bool lowPowerMode);
// void SetupDefaults();

void printGyro();
void printAccel();
void printMag();
void printTemp();
void printAttitude(float ax, float ay, float az, float mx, float my, float mz);


















//EOF
