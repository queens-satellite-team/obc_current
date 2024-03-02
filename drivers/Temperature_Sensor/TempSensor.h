#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

extern "C"
{
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <i2c/smbus.h>
}
#include <iostream>
#include <unistd.h>
#include <cmath>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <limits>
#include <bitset>
#include <cstdint>

#define MCP9808_I2CADDR_DEFAULT 0x18 ///< I2C address
#define MCP9808_REG_CONFIG 0x01      ///< MCP9808 config register

#define MCP9808_REG_CONFIG_SHUTDOWN 0x0100   ///< shutdown config
#define MCP9808_REG_CONFIG_CRITLOCKED 0x0080 ///< critical trip lock
#define MCP9808_REG_CONFIG_WINLOCKED 0x0040  ///< alarm window lock
#define MCP9808_REG_CONFIG_INTCLR 0x0020     ///< interrupt clear
#define MCP9808_REG_CONFIG_ALERTSTAT 0x0010  ///< alert output status
#define MCP9808_REG_CONFIG_ALERTCTRL 0x0008  ///< alert output control
#define MCP9808_REG_CONFIG_ALERTSEL 0x0004   ///< alert output select
#define MCP9808_REG_CONFIG_ALERTPOL 0x0002   ///< alert output polarity
#define MCP9808_REG_CONFIG_ALERTMODE 0x0001  ///< alert output mode

#define MCP9808_REG_UPPER_TEMP 0x02   ///< upper alert boundary
#define MCP9808_REG_LOWER_TEMP 0x03   ///< lower alert boundery
#define MCP9808_REG_CRIT_TEMP 0x04    ///< critical temperature
#define MCP9808_REG_AMBIENT_TEMP 0x05 ///< ambient temperature
#define MCP9808_REG_MANUF_ID 0x06     ///< manufacture ID
#define MCP9808_REG_DEVICE_ID 0x07    ///< device ID
#define MCP9808_REG_RESOLUTION 0x08   ///< resolutin

/**
 * @brief TempSensor encapsulates the temperature sensor and communication
 *
 * This class encapsulates a MCP9808 temperature sensor and provides methods for
 * reading the temperature and various communications.
 */
class TempSensor
{
public:
  /**
   * @brief Constructs a new TempSensor object
   *
   * @param busNumber int representing the i2c bus the sensor is accessible from
   */
  TempSensor(int busNumber);

  /**
   * @brief Destroys the TempSensor object
   */
  ~TempSensor();

  /**
   * @brief Reads a 16 bit value from the register
   *
   * @param reg int representing the register to check over i2c
   *
   * @return uint16 value of the 16 bits
   */
  uint16_t read16(int reg);

  /**
   * @brief Writes a 16 bit value to the register
   *
   * @param reg int representing the register to write to
   *
   * @param t uint16 representing the 16 bit value to write
   *
   * @return bool representing if succeeds
   */
  bool write16(int reg, uint16_t t);

  /**
   * @brief Reads the celsius temperature from the specified register
   *
   * @param reg int representing the register
   *
   * @return float of celsius temperature
   */
  float readTemp(int reg);

  /**
   * @brief Writes the celsius temperature to the specified register
   *
   * Not used currently
   *
   * @param reg int of register to write to
   *
   * @param temp float of celsius temperature
   *
   * @return bool representing success
   */
  bool writeTemp(int reg, float temp);

  /**
   * @brief Shuts down or starts ups the temperature sensor
   *
   * @param sw if true then shutdown, if false the startup
   *
   * @return bool representing success
   */
  bool shutdown(bool sw);

private:
  int i2cFile;
  int sensorAddress;
  int critical_value;
  int upper_value;
  int lower_value;
};

#endif