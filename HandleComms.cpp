#include "HandleComms.h"
#include <vector>

// Initialize static member
std::unordered_map<uint8_t, std::pair<uint8_t, void (*)()>> HandleComms::methodMap;

uint8_t HandleComms::read8(int reg)
{
    // Dummy method, will actually comms data from i2c
    return 0x01;
}

void HandleComms::mode_switch()
{
    int param1 = read8(0);
    int param2 = read8(1);
    std::cout << "Switching Mode" << std::endl;
}

void HandleComms::file_transfer()
{
    int param1 = read8(0);
    int param2 = read8(1);
    int param3 = read8(2);
}

void HandleComms::callFunction()
{
    // Reads function_id
    uint8_t function_id = read8(0);
    // Searches methodMap
    auto it = methodMap.find(function_id);

    if (it != methodMap.end())
    {
        // If result is not an error, then call function
        it->second.second();
    }
    else
    {
        // If result is error, print to cerr
        std::cerr << "Can't find function ID in map" << std::endl;
    }
}