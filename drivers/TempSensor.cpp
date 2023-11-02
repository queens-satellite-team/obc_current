#include "TempSensor.h"
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <iostream>

#include <map>
#include <string>

#define MCP9808_I2C_ADDRESS 0x18
#define MCP9808_REG_AMBIENT_TEMP 0x05

using namespace std;

// TempSensor::TempSensor() {}

TempSensor::TempSensor(
    bool i2c_status,
    int critical_value,
    int upper_value,
    int lower_value) : i2c_status(i2c_status),
                       critical_value(critical_value),
                       upper_value(upper_value),
                       lower_value(lower_value)
{
  // Initialize the WiringPi library
  if (wiringPiSetup() == -1)
  {
    std::cerr << "Unable to initialize WiringPi." << std::endl;
    return 1;
  }

  // Open a connection to the MCP9808 sensor
  this->mcp9808_fd = wiringPiI2CSetup(MCP9808_I2C_ADDRESS);
  if (mcp9808_fd == -1)
  {
    std::cerr << "Unable to initialize I2C communication." << std::endl;
    return 1;
  }
}

void TempSensor::reset()
{
  // reset the temperature sensor's critical upper and lower temperatures to 0
  this->critical_value = 0;
  this->upper_value = 0;
  this->lower_value = 0;
}

/**
 * Converts a floating point temperature into upper and lower bytes representing
 * an 11 bit number
 *
 * @tparam float The celsius floating point temperature
 *
 * @return pair<uint8_t, uint8_t> of an upper and lower byte representing 11-bit
 * data in two’s complement format (0.25°C)
 */
pair<uint8_t, uint8_t> TempSensor::floatToBytes(float value)
{
  int value_int = static_cast<int>(value * 16);
  int upper_byte = value_int >> 8;

  // if negative, needs to handle the signed bit
  if (value_int < 0)
  {
    upper_byte |= (1 << 2);
  }
  upper_byte &= 0x7;
  int lower_byte = value_int & 0xFF;
  return make_pair(static_cast<uint8_t>(upper_byte),
                   static_cast<uint8_t>(lower_byte));
}

/**
 * Converts an upper and lower byte representing an 11 bit number into a
 * floating point temperature
 *
 * @tparam pair<uint8_t, uint8_t> Pair of an upper and lower byte representing
 * 11-bit data in two’s complement format (0.25°C)
 *
 * @return float Returns a celsius floating point temperature
 */
float TempSensor::bytesToFloat(pair<uint8_t, uint8_t> bytes)
{
  uint16_t upper_byte = bytes.first;
  uint16_t lower_byte = bytes.second;

  if (upper_byte & (1 << 2))
  {
    upper_byte = upper_byte | 0b11111100;
    lower_byte = lower_byte;
    return (int16_t((upper_byte << 8) | lower_byte)) / 16.0;
  }

  return (int16_t((upper_byte << 8) | lower_byte)) / 16.0;
}

int TempSensor::writeTemperature(float temperature, int reg)
{
  // Convert temperature to raw data format
  int raw_temperature = static_cast<int>(temperature * 16.0);
  raw_temperature &= 0x0FFF;

  if (wiringPiI2CWriteReg16(this->mcp9808_fd, reg, raw_temperature) == -1)
  {
    std::cerr << "Failed to write temperature data." << std::endl;
    return 1;
  }

  // Successfully wrote the temperature to the service
  return 0;
}

float TempSensor::readTemperature(int reg)
{
  int raw_temperature = wiringPiI2CReadReg16(this->mcp9808_fd, MCP9808_REG_AMBIENT_TEMP);
  if (raw_temperature == -1)
  {
    std::cerr << "Failed to read temperature data." << std::endl;
    return 1;
  }

  // Convert the raw temperature data to degrees Celsius
  double temperature = (double)(raw_temperature & 0x0FFF) / 16.0;
}
