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
    SLAVE = 0x18,
    T_UPPER = 0x02,
    T_LOWER = 0x04,
    T_CRIT = 0x04,
    T_AMBIENT = 0x05,
    MANUFACTURE_ID = 0x06,
    DEVICE_ID = 0x07,
    RESOLUTION = 0x08
  };

public:
  // constructors
  TempSensor();


  TempSensor(bool i2c_status, int critical_value, int upper_value, int lower_value)
  : i2c_status(i2c_status), critical_value(critical_value), upper_value(upper_value), lower_value(lower_value){
  }

  // purpose??
  void reset(){
    // reset the temperature sensor's critical upper and lower temperatures to 0
    this->critical_value = 0;
    this->upper_value = 0;
    this->lower_value = 0;
  }


  void writeTemperature(int reg, float value){
    // writes temperature value to a register for display

    int upperByte, lowerByte;

    if(value < 0){
      int val = int(value * -16);
      upperByte = val >> 8;   // isolate upper bits
      lowerByte = val & 0xFF; // isolate lower bits

      upperByte ^= 0x0F;
      lowerByte ^= 0xFF;

      upperByte |= 0x10; // add sign bit
    }else {
      int val = int(value * 16);
      upperByte = val >> 8;   // isolate upper bits
      lowerByte = val & 0xFF; // isolate lower bits
    }

    int byteArray[2] = {upperByte, lowerByte};

    // i2c_bus.write_i2c_block_data(this->registers["slave"], reg, byteArray);

  }

  float readTemperature(int reg){
    int tempData[2] = {0x1, 0x2}; // dummy variable
    // int tempData[2] = i2c_bus.read_i2c_block_data(this->registers["slave"], reg, 2);

    int upperByte = tempData[0] & 0x1F;
    int lowerByte = tempData[1] / 16;

    if(upperByte & 0x10){ // if the 5th bit of the upper byte is a 1
      upperByte = (upperByte & 0xF) * 16;       // multiply by 16 to convert into degrees
      return -255.75 + (upperByte + lowerByte); // returns temp in celsius
    }else{
      upperByte = upperByte * 16;   // multiply
      return upperByte + lowerByte; // concatenating
    }

  }


};