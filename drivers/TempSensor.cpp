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

int TempSensor::writeTemp(int reg)
{
  return 0;
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