#ifndef handleComms_h
#define handleComms_h

#include <cstdint>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <utility>

class Comms {

public:
    static std::unordered_map<uint8_t, std::pair<int, void(*)(std::vector<int> args)> > methodMap;
    Comms();
    ~Comms();
    static uint8_t read8(int reg);
    static void modeSwitch(std::vector<int> args);
    static void fileTransfer(std::vector<int> args);
    static void callFunction();
};

#endif 