/* 
 * rtctest.cpp
 *
 * Tests the the setting and getting of all of the registers
 * in the real time clock. Also checks if the clock is ticking
 *
 */

#include "rtc.h"
#include <CppUTest/UtestMacros.h>
#include "CppUTest/CommandLineTestRunner.h"
#include <unistd.h>

TEST_GROUP(RealTimeClock){
    RTC* rtc;
    void setup(){
        rtc = new RTC();
    }
};

// Test setClock()
TEST(RealTimeClock, SetClock) {
    int in, out;

    in = 1;
    rtc->setClock(in);
    out = rtc->getClock();
    CHECK_EQUAL(in, out);

    in = 0;
    rtc->setClock(in);
    out = rtc->getClock();
    CHECK_EQUAL(in, out);
}

// Test setBattery()
TEST(RealTimeClock, SetBattery) {
    int in, out;
    
    in = 1;
    rtc->setBattery(in);
    out = rtc->getBattery();
    CHECK_EQUAL(in, out);

    in = 0;
    rtc->setBattery(in);
    out = rtc->getBattery();
    CHECK_EQUAL(in, out);
}

// Test Setting of Date time (datetime uses all the internal registers)
TEST(RealTimeClock, SettingDateTime) {
    rtc->setClock(1);
    
    std::string dateTimeIn = "5-8-2 5:3:6";
    rtc->setDateTime(dateTimeIn);

    std::string dateTimeOut = rtc->getDateTime();

    STRCMP_EQUAL(dateTimeIn.c_str(), dateTimeOut.c_str());
}

TEST(RealTimeClock, ClockTick) {
    int s0 = rtc->getSeconds();
    sleep(2);
    int s1 = rtc->getSeconds();
    int diff = s1 - s0;   
    
    int clock = rtc->getClock();
    CHECK_TRUE(clock == 1 || clock == 0);
    
    if (clock == 1) {
        CHECK_TRUE(diff > 0);
    } else if (clock == 0) {
        CHECK_EQUAL(diff, 0);
    }
}

int main(int argc, char* argv[]) {
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
