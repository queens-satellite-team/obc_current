#include "../fff.h"
#include "../../drivers/TempSensor.h"
#include <cassert>

DEFINE_FFF_GLOBALS;

// Define fake functions
FAKE_VALUE_FUNC(int, i2c_smbus_read_word_data, int, uint8_t);
FAKE_VALUE_FUNC(int, i2c_smbus_write_word_data, int, uint8_t, uint16_t);
FAKE_VALUE_FUNC_VARARG(int, snprintf, char *, size_t, const char *, ...);
FAKE_VALUE_FUNC_VARARG(int, open, const char *, int, ...);
FAKE_VALUE_FUNC_VARARG(int, ioctl, int, unsigned long int, ...);
FAKE_VALUE_FUNC(int, close, int);

void test_TempSensorConstructor_1()
{
    RESET_FAKE(snprintf);
    RESET_FAKE(open);
    RESET_FAKE(ioctl);
    RESET_FAKE(close);

    snprintf_fake.return_val = 1;
    open_fake.return_val = 2;
    ioctl_fake.return_val = 3;
    close_fake.return_val = 4;

    TempSensor sensor(1);

// makes sure snprintf was called once
    assert(snprintf_fake.call_count == 1);
    // Skip rest of snprintf, params are pointers and fixed values
    // assert(snprintf_fake.arg0_val == ???)

    assert(open_fake.call_count == 1);
    // filename never gets written to since snprintf is overridden
    // assert(open_fake.arg0_val == "/dev/i2c-1");
    assert(open_fake.arg1_val == O_RDWR);

    assert(ioctl_fake.call_count == 1);
    assert(ioctl_fake.arg0_val == 2);
    assert(ioctl_fake.arg1_val == I2C_SLAVE);
    // Variadic parameters are weird, only way to check them is define a whole function??
    // assert(ioctl_fake.arg2_val == MCP9808_I2CADDR_DEFAULT);
}

void test_TempSensorConstructor_2()
{ // TODO: Test a failed construct

//reset fake functions
  RESET_FAKE(snprintf);
  RESET_FAKE(open);
  RESET_FAKE(ioctl);
  RESET_FAKE(close);
  
  snprintf_fake.return_val = 1;     // simulates snprintf function working
  open_fake.return_val = -1;        // simulates open function failing
  
  TempSensor sensor(1);

  
  assert(snprintf_fake.call_count == 1);
  assert(open_fake.call_count == 1);
  assert(ioctl_fake.call_count == 0);
  assert(close_fake.call_count == 0);

}

void test_TempSensorDestructor_1()
{
    RESET_FAKE(snprintf);
    RESET_FAKE(open);
    RESET_FAKE(ioctl);
    RESET_FAKE(close);

    snprintf_fake.return_val = 1;
    open_fake.return_val = 2;
    ioctl_fake.return_val = 3;
    close_fake.return_val = 4;

    TempSensor sensor(1);
    sensor.~TempSensor();

    assert(close_fake.call_count == 1);
    assert(close_fake.arg0_val == 2);
}

void test_read16_1()
{
    // SetsUp i2c_smbus_read_word_data with no history and fake return
    RESET_FAKE(i2c_smbus_read_word_data);
    i2c_smbus_read_word_data_fake.return_val = 0x1234;

    // Constructs Sensor and calls read16
    TempSensor sensor(1);
    uint16_t t = sensor.read16(0x01);

    // Asserts value is correct
    assert(t == 0x3412);

    // Asserts i2c_smbus_read_word_data was called correctly
    assert(i2c_smbus_read_word_data_fake.call_count == 1);
    assert(i2c_smbus_read_word_data_fake.arg0_val == 2);
    assert(i2c_smbus_read_word_data_fake.arg1_val == 0x01);
}

void test_write16_1()
{
    RESET_FAKE(i2c_smbus_write_word_data);
    i2c_smbus_write_word_data_fake.return_val = true;

    TempSensor sensor(1);
    uint16_t t = sensor.write16(0x01, 0x1234);

    assert(i2c_smbus_write_word_data_fake.call_count == 1);
    assert(i2c_smbus_write_word_data_fake.arg0_val == 2);
    assert(i2c_smbus_write_word_data_fake.arg1_val == 0x01);
    assert(i2c_smbus_write_word_data_fake.arg2_val == 0x3412);
}

void test_readTemp_1()
{
    // Resetting the FFF call history
    RESET_FAKE(i2c_smbus_read_word_data);

    // Setting up the fake function
    i2c_smbus_read_word_data_fake.return_val = 0x65C1;

    // Creating an instance of TempSensor
    TempSensor sensor(1);

    // Call the method under test
    float temperature = sensor.readTemp(MCP9808_REG_AMBIENT_TEMP);

    // Check if the temperature is as expected
    assert(temperature == 22.3125);

    // Asserts i2c_smbus_read_word_data was called correctly
    assert(i2c_smbus_read_word_data_fake.call_count == 1);
    assert(i2c_smbus_read_word_data_fake.arg0_val == 2);
    assert(i2c_smbus_read_word_data_fake.arg1_val == 0x05);
}

void testtest_readTemp_2()
{ // TODO: test negative temp read
  RESET_FAKE(i2c_smbus_read_word_data);

  i2c_smbus_read_word_data_fake.return_val = 0x1110

  TempSensor sensor(1);
  float temperature = sensor.readTemp(MCP9808_REG_AMBIENT_TEMP);

  assert(temperature == -17);

  assert(i2c_smbus_read_word_data_fake.call_count == 1);
  assert(i2c_smbus_read_word_data_fake.arg0_val == 2);
  assert(i2c_smbus_read_word_data_fake.arg1_val == 0x05);
}

void test_shutdown_1()
{ // TODO: test shutdown true



}

void test_shutdown_2()
{ // TODO: test shutdown false


}

int main()
{
    test_TempSensorConstructor_1();
    test_TempSensorDestructor_1();
    test_read16_1();
    test_write16_1();
    test_readTemp_1();
    test_shutdown_1();

    // Add more tests as needed
    return 0;
}
