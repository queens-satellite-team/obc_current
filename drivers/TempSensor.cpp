//
// Created by hanat on 2023-10-28.
//

#include "TempSensor.h"
#include <string>
#include <iostream>
#include <map>
#include <string>

#define MCP9808_I2C_ADDRESS 0x18
#define MCP9808_REG_AMBIENT_TEMP 0x05

using namespace std;

class TempSensor{
  // Functionality:
  //  -get ambient temperature
  //  -set/get critical temperature
  //  -set/get upper temperature
  //  -set/get lower temperature

private:
  bool i2c_status;
  int critical_value;
  int upper_value;
  int lower_value;

  enum Registers {
    SLAVE = 0x1,
    T_UPPER = 0x02,
    T_LOWER = 0x03,
    T_CRIT = 0x04,
    T_AMBIENT = 0x05,
    MANUFACTURE_ID = 0x06,
    DEVICE_ID = 0x07,
    RESOLUTION = 0x08
  };

public:
  TempSensor();
  TempSensor(bool i2c_status, int critical_value, int upper_value, int lower_value)
  : i2c_status(i2c_status), critical_value(critical_value), upper_value(upper_value), lower_value(lower_value){
  }


};