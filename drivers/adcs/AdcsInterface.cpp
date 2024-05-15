#include "AdcsInterface.hpp"

#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stddef.h>

// TODO: Determine i2c bus number and write it here
const char *i2c_dev_filename = "/dev/i2c-xx";
// TODO: Find adcs i2c address
const int adcs_address = 0;
int i2c_file;

struct Coordinates {
    float latitude;
    float longitude;
};

void AdcsInterface::EnableAdcs() {
    // Set up i2c interface
    i2c_file = open(i2c_dev_filename, O_RDWR);
    if (i2c_file < 0) {
        // Error opening i2c bus
    }

    if (ioctl(i2c_file, I2C_SLAVE, adcs_address) < 0) {
        // ERROR HANDLING; you can check errno to see what went wrong
    }

    SendCommand(BOOT);
}

void AdcsInterface::PointAt(float latitude, float longitude) {
    SendCommand(INTERRUPT);
    // Wait for response from ADCS

    Coordinates coordinates = {latitude, longitude};
    SendCommandWithData(POINT, &coordinates, sizeof(coordinates));
}

void AdcsInterface::SetOrbit(TwoLineElement tle) {
    SendCommand(INTERRUPT);

    // Wait for response from ADCS

    SendCommandWithData(TLE, &tle, sizeof(tle));
}

int AdcsInterface::SendCommand(int command) {
    // Send the command over i2c
    // TODO: Check if a specific register needs to be used
    return i2c_smbus_write_byte(i2c_file, command);
}

int AdcsInterface::SendCommandWithData(int command, void* data, size_t length) {
    // TODO: Use I2C to send data
    return i2c_smbus_write_block_data(i2c_file, command, length, (__u8*)data);
}