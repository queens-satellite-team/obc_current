#ifndef handleComms_h
#define handleComms_h

#include <cstdint>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <utility>

class Comms {

public:
    // Maps ID number to function_pointer (location of code)
    // note: this definition means all methods MUST take in a parameter of type std::vector<int> called args
    static std::unordered_map<uint8_t, std::pair<int, void(*)(std::vector<int> args)> > taskMap;

    Comms();
    ~Comms();

    void Comms::handleData();

    // setters
    void Comms::setModeSwitchKey(int key);
    void Comms::setFileTransferKey(int key);

    // helper functions
    static uint8_t read8(int reg);
    static void callFunction(bool task, uint8_t function_id);

    // "tasks" available
    static void modeSwitch(std::vector<int> args);
    static void fileTransfer(std::vector<int> args);

    // "modes" available
    static void defaultMode(std::vector<int> args);
    static void mode1(std::vector<int> args);

private:
    // Maps an ID number to a satellite mode function
    // note: this definition means all modes MUST take in a parameter of type std::vector<int> called args
    static std::unordered_map<uint8_t, std::pair<int, void(*)(std::vector<int> args)> > modeMap;

};

#endif 