#include "rtc.h"
#include <unistd.h>

void useRTC(){
    RTC rtc = RTC();

    while(true) {
        sleep(100);
        printf("%d\n", rtc.getSeconds());
    }
}
