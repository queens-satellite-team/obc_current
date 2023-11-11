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

float TempSensor::readTemp(int reg)
{
  float temp = std::numeric_limits<float>::quiet_NaN();
  uint16_t t = i2c_smbus_read_word_data(i2cFile, reg);

  t = (t >> 8) | (t << 8);

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
  t = (t >> 8) | (t << 8);

  // Write the 16-bit temperature value to the register
  // return i2c_smbus_write_word_data(i2cFile, reg, t) == 0;
  return false;
}

bool TempSensor::shutdown(bool sw)
{
  uint16_t conf_shutdown;
  uint16_t conf_register = i2c_smbus_read_word_data(i2cFile, MCP9808_REG_CONFIG);

  conf_register = (conf_register >> 8) | (conf_register << 8);

  if (sw == true)
  {
    conf_shutdown = conf_register | MCP9808_REG_CONFIG_SHUTDOWN;
    conf_shutdown = (conf_shutdown >> 8) | (conf_shutdown << 8);
    i2c_smbus_write_word_data(i2cFile, MCP9808_REG_CONFIG, conf_shutdown);
  }
  if (sw == false)
  {
    conf_shutdown = conf_register & ~MCP9808_REG_CONFIG_SHUTDOWN;
    conf_shutdown = (conf_shutdown >> 8) | (conf_shutdown << 8);
    i2c_smbus_write_word_data(i2cFile, MCP9808_REG_CONFIG, conf_shutdown);
  }
  return true;
}

int main()
{
  std::cout << "Starting code" << std::endl;
  TempSensor sensor(1);

  std::cout << "Ran constructor" << std::endl;

  for (int i = 0; i < 5; i++)
  {
    std::cout << sensor.readTemp(MCP9808_REG_AMBIENT_TEMP) << std::endl;
  }
  return 0;
}