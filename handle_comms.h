#ifndef handleComms.h
#define handleComms.h

#include <cstdint>
#include <unordered_map>
#include <iostream>
#include <vector>

class Comms {

public:
    Comms();
    ~Comms();
    uint8_t read8(int reg);
    void modeSwitch();
    void fileTransfer();
    void callFunction();

};

#endif 