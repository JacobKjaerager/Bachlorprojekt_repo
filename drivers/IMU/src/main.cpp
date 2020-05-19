




#include <./Drivers/IMU/SetupIMU.h>

unsigned long lastPrint = 0;
#define PRINT_SPEED 250 // 250 ms between prints



/****************************************************************************/
/*********************  Generelle "instrukser"  *****************************/
/****************************************************************************/
/*

For at læse IMU'en tjek sæt da føsrst default værdier og start derefter IMU'en med:
  SetupIMUDefaults();
  StartIMU();

Den sidste laver objektet IMU som man så kan bruge.
Der er et sæt funktioner til hvert del element til IMU'en, dvs. når jeg henviser
til at der er en funktion
  IMU.readAccel()  /
Så antag at der også er en
  IMU.readGyro()
og
  IMU.readMag()

Tilsvarende er der data for alle akser for hvert delelement:
  IMU.ax, IMU.gx, IMU.mx
  IMU.ay, IMU.gy, IMU.my
  IMU.az, IMU.gz, IMU.mz



For at læse IMU tjek først om der er data i bufferen:
  IMU.accelerationAvailable();
Hvis den er sand så udlæs data med:
  IMU.readAccel();
Nu ligger data klar og kan bruges, for at se dem kan de tilgås på to måder:
  IMU.ax
  IMU.ay
  IMU.az
Giver de RÅ data - ved ikke om du kan bruge dem til noget.
  IMU.calcAccel(IMU.ax)
  IMU.calcAccel(IMU.ay)
  IMU.calcAccel(IMU.az)
Giver beregnede tal som er givet i hhv g (tyngde) for accelerometer,
grader pr. sec for gyroskop, og G (Gauss) for magnetometer.


Du kan bruge
  SetupACC(bool enableAcc, uint8_t scale, uint8_t sampleRate, uint8_t bandwidth )
  SetupGyro(bool enableGyr, int scale, uint8_t sampleRate, uint8_t bandwidth, bool lowPowerMode)
  SetupMag(bool enableMag, uint8_t scale, uint8_t sampleRate, bool TempCompensateEnable, bool lowPowerMode)
til at ændre på indstillinger så som sample rate osv.
Brug denne her for at indstille default indstillinger:
  SetupIMUDefaults()




*/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/




void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");


  SetupIMUDefaults();
  StartIMU();

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.println("-------");
  for (size_t i = 0; i < 600; i++) {
    PrintTest();

  }

  SetupGyro(false, 0, 0, 0, false);
  SetupMag(false, 0, 0, false, false);
  SetupACC(true,2,5,-1);

  // SetupACC(bool enableAcc, uint8_t scale, uint8_t sampleRate, uint8_t bandwidth )

}

void loop() {

  PrintTest();

}















// EOF
