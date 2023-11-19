#include "../fff.h"
#include "../../drivers/TempSensor.h"
#include <cassert>
#include <iostream>

DEFINE_FFF_GLOBALS;

// Define fake functions for FFF.h
// Replaces implementation of i2c functions
FAKE_VALUE_FUNC(int, i2c_smbus_read_word_data, int, uint8_t);
FAKE_VALUE_FUNC(int, i2c_smbus_write_word_data, int, uint8_t, uint16_t);
FAKE_VALUE_FUNC_VARARG(int, snprintf, char *, size_t, const char *, ...);
FAKE_VALUE_FUNC_VARARG(int, open, const char *, int, ...);
FAKE_VALUE_FUNC_VARARG(int, ioctl, int, unsigned long int, ...);
FAKE_VALUE_FUNC(int, close, int);

void test_TempSensorConstructor_1()
{
    std::cout << "test_TempSensorConstructor_1: Testing working constructor" << std::endl;
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
{
    std::cout << "test_TempSensorConstructor_2: Testing failed constructor" << std::endl;
    // reset fake functions
    RESET_FAKE(snprintf);
    RESET_FAKE(open);
    RESET_FAKE(ioctl);
    RESET_FAKE(close);

    snprintf_fake.return_val = 1; // simulates snprintf function working
    open_fake.return_val = -1;    // simulates open function failing

    TempSensor sensor(1);

    assert(snprintf_fake.call_count == 1);
    assert(open_fake.call_count == 1);
    assert(ioctl_fake.call_count == 0);
    assert(close_fake.call_count == 0);
}

void test_TempSensorDestructor_1()
{
    std::cout << "test_TempSensorDestructor_1: Testing destructor" << std::endl;
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
    std::cout << "test_read16_1: Testing reading 0x1234" << std::endl;
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
    std::cout << "test_write16_1: Testing writing 0x1234" << std::endl;

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
    std::cout << "test_readTemp_1: Testing reading 0x65C1 as 22.3125" << std::endl;

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

void test_readTemp_2()
{
    std::cout << "test_readTemp_2: Testing reading 0xEFFE as -17.0625" << std::endl;

    RESET_FAKE(i2c_smbus_read_word_data);

    i2c_smbus_read_word_data_fake.return_val = 0xEFFE;

    TempSensor sensor(1);
    float temperature = sensor.readTemp(MCP9808_REG_AMBIENT_TEMP);

    assert(i2c_smbus_read_word_data_fake.call_count == 1);
    assert(i2c_smbus_read_word_data_fake.arg0_val == 2);
    assert(i2c_smbus_read_word_data_fake.arg1_val == 0x05);
}

void test_shutdown_1()
{
    std::cout << "test_shutdown_1: Testing shutting up" << std::endl;
    RESET_FAKE(i2c_smbus_read_word_data);
    RESET_FAKE(i2c_smbus_write_word_data);

    i2c_smbus_read_word_data_fake.return_val = 0x0000;
    i2c_smbus_write_word_data_fake.return_val = 1;

    TempSensor sensor(1);
    sensor.shutdown(true);

    assert(i2c_smbus_read_word_data_fake.call_count == 1);
    assert(i2c_smbus_read_word_data_fake.arg0_val == 2);
    assert(i2c_smbus_read_word_data_fake.arg1_val == 0x01);

    assert(i2c_smbus_write_word_data_fake.call_count == 1);
    assert(i2c_smbus_write_word_data_fake.arg0_val == 2);
    assert(i2c_smbus_write_word_data_fake.arg1_val == 0x01);
    assert(i2c_smbus_write_word_data_fake.arg2_val == 0x0001);
}

void test_shutdown_2()
{
    std::cout << "test_shutdown_2: Testing starting up" << std::endl;
    RESET_FAKE(i2c_smbus_read_word_data);
    RESET_FAKE(i2c_smbus_write_word_data);

    i2c_smbus_read_word_data_fake.return_val = 0xFFFF;
    i2c_smbus_write_word_data_fake.return_val = 1;

    TempSensor sensor(1);
    sensor.shutdown(false);

    assert(i2c_smbus_read_word_data_fake.call_count == 1);
    assert(i2c_smbus_read_word_data_fake.arg0_val == 2);
    assert(i2c_smbus_read_word_data_fake.arg1_val == 0x01);

    assert(i2c_smbus_write_word_data_fake.call_count == 1);
    assert(i2c_smbus_write_word_data_fake.arg0_val == 2);
    assert(i2c_smbus_write_word_data_fake.arg1_val == 0x01);
    assert(i2c_smbus_write_word_data_fake.arg2_val == 0xFFFE);
}

int main()
{
    test_TempSensorConstructor_1();
    test_TempSensorConstructor_2();
    test_TempSensorDestructor_1();
    test_read16_1();
    test_write16_1();
    test_readTemp_1();
    test_readTemp_2();
    test_shutdown_1();
    test_shutdown_2();

    // Add more tests as needed
    return 0;
}
