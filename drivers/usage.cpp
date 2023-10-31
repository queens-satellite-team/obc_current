#include "TempSensor.h"
#include <unistd.h>

void useTempSensor(){
  TempSensor tempSensor = TempSensor();

  while(true) {
    usleep(10000000);
    printf("Current Temperature: %f", tempSensor.ambientTemp());
  }
}


