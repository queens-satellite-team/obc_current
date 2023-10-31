#include "TempSensor.h"
#include <string>
#include <map>

#define MCP9808_I2C_ADDRESS 0x18
#define MCP9808_REG_AMBIENT_TEMP 0x05

using namespace std;

TempSensor::TempSensor() {}

TempSensor::TempSensor(bool i2c_status, int critical_value, int upper_value, int lower_value)
: i2c_status(i2c_status), critical_value(critical_value), upper_value(upper_value), lower_value(lower_value) {

}

void TempSensor::reset(){
  // reset the temperature sensor's critical upper and lower temperatures to 0
  this->critical_value = 0;
  this->upper_value = 0;
  this->lower_value = 0;
}

/**
  * Converts a floating point temperature into upper and lower bytes representing an 11 bit number
  *
  * @tparam float The celsius floating point temperature
  *
  * @return pair<uint8_t, uint8_t> of an upper and lower byte representing 11-bit data in two’s complement format (0.25°C)
 */
pair<uint8_t, uint8_t> TempSensor::floatToBytes(float value) {
  int value_int = static_cast<int>(value * 16);
  int upper_byte = value_int >> 8;

  // if negative, needs to handle the signed bit
  if (value_int < 0) {
    upper_byte |= (1 << 2);
  }
  upper_byte &= 0x7;
  int lower_byte = value_int & 0xFF;
  return make_pair(static_cast<uint8_t>(upper_byte), static_cast<uint8_t>(lower_byte));
}

/**
 * Converts an upper and lower byte representing an 11 bit number into a floating point temperature
 *
 * @tparam pair<uint8_t, uint8_t> Pair of an upper and lower byte representing 11-bit data in two’s complement format (0.25°C)
 *
 * @return float Returns a celsius floating point temperature
 */
float TempSensor::bytesToFloat(pair<uint8_t, uint8_t> bytes) {
  uint16_t upper_byte = bytes.first;
  uint16_t lower_byte = bytes.second;

  if (upper_byte & (1 << 2)) {
    upper_byte = upper_byte | 0b11111100;
    lower_byte = lower_byte;
    return (int16_t((upper_byte << 8) | lower_byte)) / 16.0;
  }

  return (int16_t((upper_byte << 8) | lower_byte)) / 16.0;
}

void TempSensor::writeTemperature(int reg, float value){
  // Takes temperature value and writes it to the specified register as byte values
  pair<uint8_t, uint8_t> bytes = floatToBytes(value);
  uint8_t upperByte = bytes.first;
  uint8_t lowerByte = bytes.second;

  // depending on the function we use to write to the register
  // return value of floatToBytes may need to change

  // i2c_bus.write_i2c_block_data(this->registers["slave"], reg, byteArray);

}

float TempSensor::readTemperature(int reg){
  // gets the temperature value from the sensor and returns it as a float

  pair<uint8_t, uint8_t> tempData(0x1, 0x2); // dummy values
  // int tempData[2] = i2c_bus.read_i2c_block_data(this->registers["slave"], reg, 2);

  float tempValue = bytesToFloat(tempData);
  return tempValue;
}
