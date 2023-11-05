#include "rtc.h"
#include "wiringPi.h"
#include "wiringPiI2C.h"
#include <cstdint>
#include <unistd.h>
#include <ctime>
#include <iostream>
#include <stdio.h>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

extern "C" {
    #include <i2c/smbus.h>
}

/*
int main(){
    RTC* rtc = new RTC();
    while(true) {
        sleep(1);
        printf("%d\n", rtc->getSeconds());
    }
}*/

// Default Constructor
RTC::RTC() : RTC(0, 0, 0, "0-1-1 0:0:0") {}

// Constructor that sets the specified values in the rtc
RTC::RTC(int battery, int clock, int i2c_status, std::string datetime) 
    : i2c_status(i2c_status) {
    
    this->fd = initI2C();

    this->setBattery(battery);
    this->setClock(clock);
    this->setDateTime(datetime);
}   

// resets the rtc to default values
int RTC::reset() {
    this->setBattery(0);
    this->setClock(0);
    this->setDateTime("0-1-1 0:0:0");
    this->i2c_status = 0;
    return 0;
}

int RTC::initI2C(){
    int file = open("/dev/i2c-2", O_RDWR);
    
    if (file < 0) {
        perror("Failed to open the i2c bus");
        exit(1);
    }

    if (ioctl(file, I2C_SLAVE, RTC_SLAVE)) {
        perror("Failed to acqurie bus access and/or talk to slave\n");
        exit(1);
    }
    
    return file;   
}

// Verify that the clock is working as intended
int RTC::checkTick(int clock) {
    
    int s0 = this->getSeconds();
    sleep(5);
    int s1 = this->getSeconds();
    int diff = s1 - s0;   
    
    if (clock == 1) {
        if (diff > 0) {
            std::cout << "Success: Clock is Ticking" << std::endl;
            return 0;
        }else {
            std::cout << "Fail: Clock is not Ticking" << std::endl;
            return -1;
        }
    } else if (clock == 0) {
        if (diff == 0) {
            std::cout << "Success: Clock is not Ticking" << std::endl;
            return 0;
        }else {
            std::cout << "Fail: Clock is Ticking" << std::endl;
            return -1;
        }
    } else {
        std::cerr << "Clock State Invalid" << std::endl;
    }
    return -1;
}

// Get the byte at the specified register from the rtc
int RTC::getRegister(Register reg) {
    int data = i2c_smbus_read_byte_data(this->fd, reg);
    
    if (data == -1) {
        std::cerr << "Unable to get Register: " << reg << std::endl;
    }
    
    return data;
}

//Gets the clock status bit
int RTC::getClock() {
    int status = this->getRegister(SEC) & 0b1000000;
    return status >> 7;
}

//Gets the backup battery enable bit
int RTC::getBattery() {
    int value = this->getRegister(WEEKDAY);
    int battery = (value & 0b1000) >> 3;
    return battery;
}

//Gets the current seconds 
int RTC::getSeconds() {
    int value = this->getRegister(SEC) & 0b01111111;
    int seconds = (value >> 4) * 10 + value & 0b1111;
    return seconds;
}

//Gets the current minutes
int RTC::getMinutes() {
    int value = this->getRegister(MIN) & 0b01111111;
    int minutes = (value >> 4) * 10 + value & 0b1111;
    return minutes;
}

//Gets the current Hours
int RTC::getHours() {
    int value = this->getRegister(HOUR) & 0b00111111;
    return (value >> 4) * 10 + value & 0b1111;
}

//Gets the current Weekday //is this needed?
int RTC::getWeekDay() {
    int value = this->getRegister(WEEKDAY) & 0b0111;
    return value;
}

//Gets the current Date in the month
int RTC::getDate() {
    int value = this->getRegister(DATE) & 0b00111111;
    return (value >> 4) * 10 + value & 0xF;
}

//Gets the current month in the year
int RTC::getMonth() {
    int value = this->getRegister(MONTH) & 0b00011111;
    return (value >> 4) * 10 + value & 0xF;
}

//Gets the current year - 2000
int RTC::getYear() {
    int value = this->getRegister(YEAR) & 0b11111111;
    return (value >> 4) * 10 + value & 0xF;
}

//Formats the internal values into a string
//Yyyy-mm-dd h:m:s
std::string RTC::getDateTime() {
    int sec = this->getSeconds();
    int min = this->getMinutes();
    int hour = this->getHours();
    int date = this->getDate();
    int month = this->getMonth();
    int year = this->getYear() + 2000;
    char datetime[50];
    std::sprintf(datetime, "%d-%d-%d %d:%d:%d", year, month, date, hour, min, sec);
    return datetime;
}

//Sets an internal register with a byte
int RTC::setRegister(Register reg, __u8 byte) {
    int write = i2c_smbus_write_byte_data(this->fd, reg, byte);
    i2c_status = write;
    return write;
}

//Sets the battery bit to the specified state
int RTC::setBattery(int state) {
    __u8 value = this->getRegister(WEEKDAY);
    if (state) {
        value |= 0b00001000;
    } else {
        value &= 0b11110111;
    }

    int write = this->setRegister(WEEKDAY, value);
    return write;
}

//Sets the clock bit to the specified state
int RTC::setClock(int state) {
    __u8 value = this->getRegister(SEC);
    
    if (state) {
        value |= 0b10000000;
    } else {
        value &= 0b01111111;
    }
    
    int write = this->setRegister(SEC, value); 
    this->checkTick(state);
    return write;
}

//Sets the seconds bits to the specified value
int RTC::setSeconds(int value) {
    if (verifyDate(SEC, value)) {
        __u8 enc = RTC::encodeDecimal(value);
        __u8 clock = this->getClock();
        __u8 byte = (clock << 7) | enc;
        return this->setRegister(SEC, byte);
    }
    return -1;
}

//Sets the minutes bits to the specified value
int RTC::setMinutes(int value) {
    if (verifyDate(MIN, value)) {
        __u8 enc = RTC::encodeDecimal(value);
        return this->setRegister(MIN, enc);
    }
    return -1;
}

//Sets the hours bits
int RTC::setHours(int value) {
    if (verifyDate(HOUR, value)) {
        __u8 raw = this->getRegister(HOUR);
        __u8 enc = RTC::encodeDecimal(value);
        __u8 byte = (raw & 0b01000000) | enc;
        return this->setRegister(HOUR, enc);
    }
    return -1;
}

//Sets the weekday
int RTC::setWeekDay(int value) {
    if (verifyDate(WEEKDAY, value)) {
        __u8 raw = this->getRegister(WEEKDAY);
        __u8 enc = RTC::encodeDecimal(value);
        __u8 byte = (raw & 0b00100000) | enc;
        return this->setRegister(WEEKDAY, enc);
    }
    return -1;
}

//Sets the date
int RTC::setDate(int value) {
    if (verifyDate(DATE, value)) {
        __u8 enc = RTC::encodeDecimal(value);
        return this->setRegister(WEEKDAY, enc);
    }
    return -1;
}

//Sets the month
int RTC::setMonth(int value) {
    if (verifyDate(MONTH, value)) {
        __u8 raw = this->getRegister(MONTH);
        __u8 enc = RTC::encodeDecimal(value);
        __u8 byte = (raw & 0b11100000) | enc;
        return this->setRegister(MONTH, enc);
    }
    return -1;
}

//Sets the year
int RTC::setYear(int value) {
    if (verifyDate(YEAR, value)) {
        __u8 enc = RTC::encodeDecimal(value - 2000);
        return this->setRegister(YEAR, enc);
    }
    return -1;
}

// Format is '%Y-%m-%d %H:%M:%S'
int RTC::setDateTime(std::string time) {
    struct tm tm;
    if(strptime(time.c_str(), "%Y-%m-%d %H:%M:%S", &tm)){
        this->setSeconds(tm.tm_sec);
        this->setMinutes(tm.tm_min);
        this->setHours(tm.tm_hour);
        this->setDate(tm.tm_mday);
        this->setMonth(tm.tm_mon);
        this->setYear(tm.tm_year + 1900); //tm year = year - 1900
        this->i2c_status = 0;
        return 0;
    }
    this->i2c_status = 1;
    return -1;
}

// Sets the date tiem using the calender date and time struct from c
int RTC::setDateTime(struct tm tm) {
    this->setSeconds(tm.tm_sec);
    this->setMinutes(tm.tm_min);
    this->setHours(tm.tm_hour);
    this->setDate(tm.tm_mday);
    this->setMonth(tm.tm_mon);
    this->setYear(tm.tm_year + 1900); //tm year = year - 1900
    this->i2c_status = 0;
    return 1;
}

__u8 RTC::encodeDecimal(int value) {
    __u8 ones = value % 10;
    __u8 tens = uint8_t(value / 10);
    return (tens << 4) + ones;
}

bool RTC::verifyDate(Register reg, int value) {
    if (reg == SEC)
        return 0 <= value && value <= 60;
    if (reg == MIN)
        return 0 <= value && value <= 60;
    if (reg == HOUR)
        return 0 <= value && value <= 24;
    if (reg == DATE)
        return 1 <= value && value <= 31;
    if (reg == MONTH)
        return 1 <= value && value <= 12;
    if (reg == YEAR)
        return 0 <= value && value <= 99;

    return false;
}
