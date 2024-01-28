#include <unordered_map>
#include <iostream>
#include <cstdint>
#include <vector>
#include "handle_comms.h"

// This code maps function IDs to fuctions using function pointers and unordered maps

// Maps function_id to function_pointer (location of code)
std::unordered_map<uint8_t, std::pair<int, void(*)()> > Comms::methodMap; // unordered map named methodMap, maps uint8_t keys to function pointers. The function pointers are of type void(*)(), meaning they point to functions that take no arguments and return void.

Comms::Comms(){}
Comms::~Comms(){}

uint8_t Comms::read8(int reg) {
    // This function is a placeholder for reading data, will actually be comms data from i2c
    return 0x01;
}

//commands available:
void Comms::modeSwitch(){
    //Example usage of this function: methodMap[1] = &modeSwitch; 
    //                                methodMap[2] = &fileTransfer;
    //                                callFunction();
    // Each command reads in its own data
    int param1 = read8(0);
    int param2 = read8(1);
    std::cout << "Switching Mode" << std::endl;
}

void Comms::fileTransfer(){
    int param1 = read8(0);
    int param2 = read8(1);
    int param3 = read8(2);
}


void Comms::callFunction() {
    // Function is responsible for looking up a function pointer in methodMap based on a given function ID obtained from read8(0). If it finds a matching function, it calls it. Otherwise, it reports an error.
    // Reads function_id
    uint8_t function_id = read8(0);
    // Searches methodMap
    auto it = methodMap.find(function_id);

    if (it != methodMap.end()) {
        // If result is not an error, then call function
        int numArgs = it->second.first;

        for(int i=0; i < numArgs; i++){
            //read additional arguments based on the functions argument count? maybe?
            int args = read8(i + 1);
        }

        it->second.second();
        

    } else {
        // If result is error, print to cerr
        std::cerr << "Can't find function ID in map" << std::endl;
    }
}
 int main() {
    // Create an instance of the Comms class
    // Comms commsInstance;

    // Define function mappings
    Comms::methodMap[1] = std::make_pair(2, &Comms::modeSwitch);
    Comms::methodMap[2] = std::make_pair(3, &Comms::fileTransfer);

    // Test calling a function
    Comms::callFunction();

    return 0;
}
