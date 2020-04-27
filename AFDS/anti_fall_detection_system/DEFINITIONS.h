
#include <time.h>

struct AXES{
  float x;
  float y;
  float z;
};

typedef struct{
  int         sampleNbr;
  time_t      seconds;
  AXES        acc;
  AXES        gyr;
  AXES        mag;
} IMU_VALS;
