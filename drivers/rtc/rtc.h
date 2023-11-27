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
#include <cstdint>

#define RTC_SLAVE 0b01101111 // 0x6F
#define RTC_DEVICE_PATH "/dev/i2c-%d"
#define DEFAULT_TIME "0-1-1 0:0:0"
#define TIME_FORMAT "%Y-%m-%d %H:%M:%S"

#define I2C_OPEN 0
#define I2C_REGERROR -1
#define I2C_CLOSED -1

int initI2C(int* fd, int adapter);

class RTC {
    // Addresses for Internal Registers
public: 
    enum Register : uint8_t {
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
    // Initializes RTC registers to the specified values
    RTC(int adapter);
    RTC(int battery, int clock, std::string datetime, int adapter);
    ~RTC();

    int init(int adapter); // takes in i2c adapter for rtc
    int releaseFd(); // closes the file
    int reset(); // reset the clock registers

    void print(); // Prints the current datetime

    // Get the Specified Values from the real time clock
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

    // Set the specified register/bits to the passed in value
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
    
    // 1 => ON, s <= 0 => OFF  
    int i2c_status;

private:

    /*
     *  Encodes the passed in value into an unsigned 8-bit integer such that 
     *  the low bits of the byte represents the ones digit
     *  and the high bits of the byte represents the tens
     */
    static uint8_t encodeDecimal(int value);
    int setRegister(Register reg, uint8_t byte); // Writes a byte to an internal register
    int getRegister(Register reg); // Reads a byte from an internal register
    bool verifyDate(Register reg, int value); // Verifies that a time is valid
    
    // File Descriptor
    int fd;

};


#endif
