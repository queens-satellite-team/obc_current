#ifndef OBC_RTC_H
#define OBC_RTC_H

#include <string>

class RTC {

    enum Register {
        SLAVE = 0x6F,
        SEC = 0x00,
        MIN = 0x01,
        HOUR = 0x02,
        WEEKDAY = 0x03,
        DATE = 0x04,
        MONTH = 0x05,
        YEAR = 0x06,
    };

public:
    RTC();
    RTC(int battery, int clock, int i2c_status, std::string datetime);

    int reset();

    int getSeconds();
    int getHours();
    int getMinutes();
    int getWeekDay();
    int getDate();
    int getMonth();
    int getYear();
    std::string getDateTime();

    int setSeconds(int seconds);
    int setHours(int hours);
    int setMinutes(int minutes);
    int setWeekDay(int weekDay);
    int setDate(int date);
    int setMonth(int month);
    int setYear(int year);
    int setDateTime(std::string time);
    
    int setBattery(bool state);
    int getBattery();

    int setClock(bool state);
    int getClock();

private:
    static uint8_t encodeDecimal(int value);
    int checkTick();

    int setRegister(Register reg, uint8_t byte);
    int getRegister(Register reg);

    bool verifyDate(Register reg, int value);
    
    int i2c_status;
    int fd;

};


#endif
