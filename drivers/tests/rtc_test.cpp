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

int main() {
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}
