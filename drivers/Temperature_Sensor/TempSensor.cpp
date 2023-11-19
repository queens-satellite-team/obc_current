#include "TempSensor.h"

TempSensor::TempSensor(int busNumber)
{
  // Open the I2C bus
  char filename[20];
  snprintf(filename, sizeof(filename), "/dev/i2c-%d", busNumber);
  i2cFile = open(filename, O_RDWR);
  if (i2cFile < 0)
  {
    std::cerr << "Failed to open the I2C bus." << std::endl;
    return;
  }

  // Set the I2C device address
  sensorAddress = MCP9808_I2CADDR_DEFAULT;
  if (ioctl(i2cFile, I2C_SLAVE, sensorAddress) < 0)
  {
    std::cerr << "Failed to set I2C device address." << std::endl;
    close(i2cFile);
    return;
  }
}

TempSensor::~TempSensor()
{
  // Close the I2C bus when the object is destroyed
  if (i2cFile >= 0)
  {
    close(i2cFile);
  }
}

uint16_t TempSensor::read16(int reg)
{
  uint16_t t = i2c_smbus_read_word_data(i2cFile, reg);
  t = (t >> 8) | (t << 8);
  return t;
}

bool TempSensor::write16(int reg, uint16_t t)
{
  t = (t >> 8) | (t << 8);
  i2c_smbus_write_word_data(i2cFile, reg, t);
  return true;
}

float TempSensor::readTemp(int reg)
{
  float temp = std::numeric_limits<float>::quiet_NaN();
  uint16_t t = read16(reg);

  // std::bitset<16> binary(t);
  // std::cout << binary << std::endl;

  if (t != 0xFFFF)
  {
    temp = t & 0x0FFF;
    temp /= 16.0;
    if (t & 0x1000)
      temp -= 256;
  }

  return temp;
}

bool TempSensor::writeTemp(int reg, float temp)
{
  // Untested code: GL
  if (temp < -256.0 || temp > 255.875)
  {
    // Temperature out of range that can be represented
    return false;
  }

  // Convert temperature to a 16-bit value
  uint16_t t;
  if (temp < 0)
  {
    temp += 256;
    t = static_cast<uint16_t>(temp * 16.0) & 0x0FFF;
    t |= 0x1000; // Set sign bit for negative temperatures
  }
  else
  {
    t = static_cast<uint16_t>(temp * 16.0) & 0x0FFF;
  }

  // Correct byte order for I2C transmission

  // Write the 16-bit temperature value to the register
  // return write16(i2cFile, reg, t) == 0;
  return false;
}

bool TempSensor::shutdown(bool sw)
{
  uint16_t conf_shutdown;
  uint16_t conf_register = read16(MCP9808_REG_CONFIG);

  if (sw == true)
  {
    conf_shutdown = conf_register | MCP9808_REG_CONFIG_SHUTDOWN;
    write16(MCP9808_REG_CONFIG, conf_shutdown);
  }
  if (sw == false)
  {
    conf_shutdown = conf_register & ~MCP9808_REG_CONFIG_SHUTDOWN;
    write16(MCP9808_REG_CONFIG, conf_shutdown);
  }
  return true;
}