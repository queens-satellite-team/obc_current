#include <unordered_map>
#include <iostream>
#include <cstdint>
#include <vector>

// This code maps function IDs to fuctions using function pointers and unordered maps

// Maps function_id to function_pointer (location of code)
std::unordered_map<uint8_t, void(*)()> methodMap; // unordered map named methodMap, maps uint8_t keys to function pointers. The function pointers are of type void(*)(), meaning they point to functions that take no arguments and return void.

uint8_t read8(int reg) {
    // This function is a placeholder for reading data, will actually be comms data from i2c
    return 0x01;
}

//commands available:
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
    // Function is responsible for looking up a function pointer in methodMap based on a given function ID obtained from read8(0). If it finds a matching function, it calls it. Otherwise, it reports an error.
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