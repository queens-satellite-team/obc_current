#include "TempSensor.h"
#include <cstdio>
#include <unistd.h>

using namespace std;

int main()
{
  std::printf("Starting usage.cpp\n");
  TempSensor tempSensor(0, 0, 0, 0);

  std::printf("Ran tempSensor Constructor\n");

  while (true)
  {
    std::printf("Current Temperature: %f", tempSensor.ambientTemp());
    usleep(10000000);
  }
  return 0;
}
// 110 1011 1100 0001