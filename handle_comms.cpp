#include <unordered_map>
#include <iostream>
#include <cstdint>
#include <vector>

std::unordered_map<uint8_t, void(*)()> methodMap;

uint8_t read8(int reg) {
    // Dummy method, will actually comms data from i2c
    return 0x01;
}

void mode_switch(){
    // Each command reads in its own data
    int param1 = read8(0);
    int param2 = read8(1);
    std::cout << "Switching Mode" << std::endl;
}

void file_transfer(){
    int param1 = read8(0);
    int param2 = read8(1);
    int param3 = read8(2);
}


void callFunction() {
    // Reads function_id
    uint8_t function_id = read8(0);
    // Searches methodMap
    auto it = methodMap.find(function_id);

    if (it != methodMap.end()) {
        // If result is not an error, then call function
        it->second();
    } else {
        // If result is error, print to cerr
        std::cerr << "Can't find function ID in map" << std::endl;
    }
}

int main() {
    // Maps function_id (uint8_t) to function pointer
    methodMap[1] = &mode_switch;
    methodMap[2] = &file_transfer;

    callFunction();
}