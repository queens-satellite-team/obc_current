/*
 *  rtc.h
 *
 *  Driver for MCP679410
 *  Data Sheet: http://ww1.microchip.com/downloads/en/devicedoc/20002266h.pdf
 *
 *  Functionality:
 *      -Start and Stop Clock
 *      -Get date and time from clock
 *      -enable/disable backup battery
 *      -reset clock
 * 
 */

#ifndef OBC_RTC_H
#define OBC_RTC_H

#include <string>

#define SLAVE 0x6F

class RTC {
    
    // Addresses for Internal Registers
    enum Register {
        SEC = 0x00,
        MIN = 0x01,
        HOUR = 0x02,
        WEEKDAY = 0x03,
        DATE = 0x04,
        MONTH = 0x05,
        YEAR = 0x06,
    };

public:

    // Default constructor
    RTC();

    /*
     *  Constructor for Real time Clock
     *
     *  Initializes RTC registers to the specified values
     */
    RTC(int battery, int clock, int i2c_status, std::string datetime);
    
    // Resets RTC
    int reset();
    

    /*
     *  Getters
     *
     *  Get the Specified Values from the real time clock
     */
    int getBattery();
    int getClock();
    int getSeconds();
    int getHours();
    int getMinutes();
    int getWeekDay();
    int getDate();
    int getMonth();
    int getYear();
    std::string getDateTime();    

    /*
     *  Setters
     *  
     *  Set the specified register/bits to the passed in value
     *
     */
    int setBattery(int state);
    int setClock(int state);
    int setSeconds(int seconds);
    int setHours(int hours);
    int setMinutes(int minutes);
    int setWeekDay(int weekDay);
    int setDate(int date);
    int setMonth(int month);
    int setYear(int year);
    int setDateTime(std::string time);
    int setDateTime(struct tm tm);
    
private:

    /*
     *  Encodes the passed in value into an unsigned 8-bit integer such that 
     *  the low bits of the byte represents the ones digit
     *  and the high bits of the byte represents the tens
     */
    static uint8_t encodeDecimal(int value);
    

    // Checks if the clock is running as it's supposed to in the proper state
    int checkTick(int clock);
    
    // Writes a byte to an internal register
    int setRegister(Register reg, uint8_t byte);

    // Reads a byte from an internal register
    int getRegister(Register reg);

    // Verifies that the passed in value for setting the date and time is valid
    bool verifyDate(Register reg, int value);
    
    int i2c_status;
    int fd;

};


#endif
