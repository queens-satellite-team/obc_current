//
// Created by hanat on 2023-10-28.
//
#include "drivers/rtc.h"

int main() { 
    RTC* rtc = new RTC(0, 1, 0, "0-1-1 0:0:0");
    while(true){
        printf("%d\n", rtc->getSeconds());
    }
    return 0;
};
