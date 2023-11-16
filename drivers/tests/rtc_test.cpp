#include "rtc.h"
#include "gtest/gtest.h"

// Test setClock()
TEST(RTC, SetClock) {
    RTC* rtc = new RTC();

    int in = 1;
    rtc->setClock(in);
    int out = rtc->getClock();
    EXPECT_EQ(in, out);

    in = 0;
    rtc->setClock(in);
    out = rtc->getClock();
    EXPECT_EQ(in, out);
}

// Test setBattery()
TEST(RTC, SetBattery) {
    RTC* rtc = new RTC();
    int in = 1;
    rtc->setBattery(in);
    int out = rtc->getBattery();
    EXPECT_EQ(in, out);

    in = 0;
    rtc->setBattery(in);
    out = rtc->getBattery();
    EXPECT_EQ(in, out);
}

// Test Setting of Date time (datetime uses all the internal registers)
TEST(RTC, SettingDateTime) {
    RTC* rtc = new RTC();
    rtc->setClock(1);
    
    std::string dateTimeIn = "5-8-2 5:3:6";
    rtc->setDateTime(dateTimeIn);

    std::string dateTimeOut = rtc->getDateTime();

    EXPECT_STREQ(dateTimeIn.c_str(), dateTimeOut.c_str());
}

TEST(RTC, ClockTick) {
    RTC* rtc = new RTC();
    
    int s0 = rtc->getSeconds();
    sleep(2);
    int s1 = rtc->getSeconds();
    int diff = s1 - s0;   
    
    int clock = rtc->getClock();
    EXPECT_TRUE(clock == 1 || clock == 0);
    
    if (clock == 1) {
        EXPECT_GT(diff, 0);
    } else if (clock == 0) {
        EXPECT_EQ(diff, 0);
    }
}

int main() {
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}
