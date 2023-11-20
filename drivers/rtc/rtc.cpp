#include "rtc.h"
#include <iostream>
#include <stdio.h>
#include <bitset>

#include <ctime>
#include <unistd.h>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

extern "C" {
    #include <i2c/smbus.h>
}

int initI2C(int* fd, int adapter){
    char filename[20];
    snprintf(filename, 19, RTC_DEVICE_PATH, adapter);
    
    *fd = open(filename, O_RDWR);  
    if (*fd < 0) {
        perror("Failed to open the i2c bus");
        return -1;
    }
    
    int status = ioctl(*fd, I2C_SLAVE, RTC_SLAVE);
    if (status < 0) {
        perror("Failed to acqurie bus access and/or talk to slave\n");
        return -1;
    }
    
    printf("INFO: Successfullly Initialized i2c\n");

    return status;   
}

// Default Constructor
RTC::RTC() : RTC(0, 0, "0-1-1 0:0:0", 0) {}

RTC::RTC(int adapter) : RTC(0, 0, "0-1-1 0:0:0", adapter) {}

// Constructor that sets the specified values in the rtc
RTC::RTC(int battery, int clock, std::string datetime, int adapter){    
    if (this->init(adapter)) {
        this->setBattery(battery);
        this->setClock(clock);
        this->setDateTime(datetime);
    }
}   

RTC::~RTC() {
    this->releaseFd();
}

int RTC::init(int adapter) {
    int status = initI2C(&this->fd, adapter);

    if (status < 0) {
        this->i2c_status = I2C_CLOSED;
        perror("Could not initialize real time clock");
        return -1;
    }

    this->i2c_status = I2C_OPEN;
    printf("INFO: Successfully initialized real time clock");
    
    return 1;
}

int RTC::releaseFd() {
    this->i2c_status = I2C_CLOSED;

    int status = close(this->fd);
    
    if (status < 0) {
        perror("Could not close file descriptor");
    } else {
        printf("INFO: Closed file descripter for RTC\n");
    }

    return status;
}

// resets the rtc to default values
int RTC::reset() {
    this->setBattery(0);
    this->setClock(0);
    this->setDateTime("0-1-1 0:0:0");
    return 0;
}

void RTC::print(){
    std::string datetime = this->getDateTime();
    std::cout << datetime << "\n";
}

// Get the byte at the specified register from the rtc
int RTC::getRegister(Register reg) {
    
    if (this->i2c_status != I2C_OPEN){
        return -1;
    }

    int data = i2c_smbus_read_byte_data(this->fd, reg);

    if (data == -1) {
        std::cerr << "Unable to get Register: " << reg << std::endl;
    }

    return data;
}

//Gets the clock status bit
int RTC::getClock() {
    int status = this->getRegister(SEC) & 0b10000000;
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
int RTC::setRegister(Register reg, uint8_t byte) {
    
    if (this->i2c_status != I2C_OPEN){
        return -1;
    }

    int status = i2c_smbus_write_byte_data(this->fd, reg, byte);
    return status;
}

//Sets the battery bit to the specified state
int RTC::setBattery(int state) {
    uint8_t value = this->getRegister(WEEKDAY);
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
    uint8_t value = this->getRegister(SEC);

    if (state) {
        value |= 0b10000000;
    } else {
        value &= 0b01111111;
    }

    int write = this->setRegister(SEC, value);
    return write;
}

//Sets the seconds bits to the specified value
int RTC::setSeconds(int value) {
    if (verifyDate(SEC, value)) {
        uint8_t enc = RTC::encodeDecimal(value);
        uint8_t clock = this->getClock();
        uint8_t byte = (clock << 7) | enc;
        return this->setRegister(SEC, byte);
    }
    return -1;
}

//Sets the minutes bits to the specified value
int RTC::setMinutes(int value) {
    if (verifyDate(MIN, value)) {
        uint8_t enc = RTC::encodeDecimal(value);
        return this->setRegister(MIN, enc);
    }
    return -1;
}

//Sets the hours bits
int RTC::setHours(int value) {
    if (verifyDate(HOUR, value)) {
        uint8_t raw = this->getRegister(HOUR);
        uint8_t enc = RTC::encodeDecimal(value);
        uint8_t byte = (raw & 0b01000000) | enc;
        return this->setRegister(HOUR, enc);
    }
    return -1;
}

//Sets the weekday
int RTC::setWeekDay(int value) {
    if (verifyDate(WEEKDAY, value)) {
        uint8_t raw = this->getRegister(WEEKDAY);
        uint8_t enc = RTC::encodeDecimal(value);
        uint8_t byte = (raw & 0b00100000) | enc;
        return this->setRegister(WEEKDAY, enc);
    }
    return -1;
}

//Sets the date
int RTC::setDate(int value) {
    if (verifyDate(DATE, value)) {
        uint8_t enc = RTC::encodeDecimal(value);
        return this->setRegister(WEEKDAY, enc);
    }
    return -1;
}

//Sets the month
int RTC::setMonth(int value) {
    if (verifyDate(MONTH, value)) {
        uint8_t raw = this->getRegister(MONTH);
        uint8_t enc = RTC::encodeDecimal(value);
        uint8_t byte = (raw & 0b11100000) | enc;
        return this->setRegister(MONTH, enc);
    }
    return -1;
}

//Sets the year
int RTC::setYear(int value) {
    if (verifyDate(YEAR, value)) {
        uint8_t enc = RTC::encodeDecimal(value - 2000);
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
        return 0;
    }
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
    return 1;
}

uint8_t RTC::encodeDecimal(int value) {
    uint8_t ones = value % 10;
    uint8_t tens = uint8_t(value / 10);
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
