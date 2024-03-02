#ifndef HANDLECOMMS_H
#define HANDLECOMMS_H

#include <unordered_map>
#include <cstdint>
#include <iostream>

class HandleComms
{
public:
    static std::unordered_map<uint8_t, std::pair<uint8_t, void (*)()>> methodMap;

    static uint8_t read8(int reg);
    static void mode_switch();
    static void file_transfer();
    static void callFunction();
};

#endif // HANDLECOMMS_H
