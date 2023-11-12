//
// Created by hanat on 2023-10-28.
//
#include "rtc.h"
#include <unistd.h>
#include <string>
#include <iostream>

int main() { 
    RTC* rtc = new RTC(0, 1, 0, "0-1-1 0:0:0");
    while(true) {
        sleep(1);
        
        rtc->print();
    }
    return 0;
};
