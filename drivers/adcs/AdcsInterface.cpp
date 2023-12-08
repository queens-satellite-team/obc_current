#include "AdcsInterface.hpp"

#include <stddef.h>

struct Coordinates {
    float latitude;
    float longitude;
};

void AdcsInterface::EnableAdcs() {
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

void AdcsInterface::SendCommand(int command) {
    // TODO: Use I2C to send data
}

void AdcsInterface::SendCommandWithData(int command, void* data, size_t length) {
    // TODO: Use I2C to send data
}