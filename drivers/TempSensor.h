#ifndef OBC_CURRENT_TEMP_SENSOR_H
#define OBC_CURRENT_TEMP_SENSOR_H

#include <string>
#include <iostream>
#include <map>
#include <string>
#include <cassert>
#include <bitset>

using namespace std;

class TempSensor{
public:
  // Constructor
  TempSensor();
  TempSensor(bool i2c_status, int critical_value, int upper_value, int lower_value);

  // Member functions
  void reset();
  pair<uint8_t, uint8_t> floatToBytes(float value);
  float bytesToFloat(pair<uint8_t, uint8_t> bytes);
  void writeTemperature(int reg, float value);
  float readTemperature(int reg);

private:
  // Attributes
  bool i2c_status;
  int critical_value;
  int upper_value;
  int lower_value;

  enum Registers {
    SLAVE = 0x18,
    T_UPPER = 0x02,
    T_LOWER = 0x04,
    T_CRIT = 0x04,
    T_AMBIENT = 0x05,
    MANUFACTURE_ID = 0x06,
    DEVICE_ID = 0x07,
    RESOLUTION = 0x08
  };
};


#endif // OBC_CURRENT_TEMP_SENSOR_H
