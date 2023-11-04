//
// Created by hanat on 2023-10-28.
//
#include "drivers/rtc.h"
#include <unistd.h>
#include <string>
#include <iostream>

int main() { 
    RTC* rtc = new RTC(0, 1, 0, "0-1-1 0:0:0");
    while(true) {
        sleep(1);
        std::string datetime = rtc->getDateTime();
        std::cout << datetime << "\n";
    }
    return 0;
};
