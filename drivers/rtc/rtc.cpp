#include "rtc.h"
#include "wiringPi.h"
#include "wiringPiI2C.h"
#include <unistd.h>
#include <ctime>
#include <iostream>

RTC::RTC() : RTC(1, 1, 0, "2000-10-13 2:54:31") {
    if (wiringPiSetup() == -1) {
        std::cerr << "Unable to initialize WiringPi." << std::endl;
    }

    this->fd = wiringPiI2CSetup(SLAVE);

    if (this->fd== -1) {
        std::cerr << "Unable to initialize I2C." << std::endl;
    }

    this->setBattery(this->battery);
    this->setClock(this->clock);
    this->setDateTime(this->dateTime);
}

RTC::RTC(int battery, int clock, int i2c_status, std::string datetime) 
    : battery(battery),  clock(clock), i2c_status(i2c_status), dateTime(datetime) {
    
}

int RTC::reset() {
    this->battery = this->getBattery();
    this->clock = this->getClock();

    return 0;
}

int RTC::checkTick() {
    
    int s0 = this->getSeconds();
    sleep(5);
    int s1 = this->getSeconds();
    int diff = s1 - s0;   

    if (this->clock == 1) {
        if (diff > 0) {
            std::cout << "Success: Clock is Ticking" << std::endl;
            return 0;
        }else {
            std::cout << "Fail: Clock is not Ticking" << std::endl;
            return -1;
        }
    } else if (this->clock == 0) {
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

int RTC::getRegister(Register reg) {
    const int value = wiringPiI2CReadReg8(this->fd, reg);
    
    if (value == -1) {
        std::cerr << "Unable to get Register: " << reg << std::endl;
    }
    
    return value;
}

int RTC::getClock() {
    const int status = wiringPiI2CReadReg8(this->fd, SEC) & 0b1000000;
    return status >> 7;
}

int RTC::getBattery() {
    const int value = wiringPiI2CReadReg8(this->fd, WEEKDAY);
    const int battery = (value & 0b1000) >> 3;

    return battery;
}

int RTC::getSeconds() {
    const int value = this->getRegister(SEC) & 0b01111111;
    const int seconds = (value >> 4) * 10 + value & 0b1111;
    return seconds;
}

int RTC::getMinutes() {
    const int value = this->getRegister(MIN) & 0b01111111;
    const int minutes = (value >> 4) * 10 + value & 0b1111;
    return minutes;
}

int RTC::getHours() {
    const int value = this->getRegister(HOUR) & 0b00111111;
    return (value >> 4) * 10 + value & 0b1111;
}

int RTC::getWeekDay() {
    const int value = this->getRegister(WEEKDAY) & 0b0111;
    return value;
}

int RTC::getDate() {
    const int value = this->getRegister(DATE) & 0b00111111;
    return (value >> 4) * 10 + value & 0xF;
}

int RTC::getMonth() {
    const int value = this->getRegister(MONTH) & 0b00011111;
    return (value >> 4) * 10 + value & 0xF;
}

int RTC::getYear() {
    const int value = this->getRegister(YEAR) & 0b11111111;
    return (value >> 4) * 10 + value & 0xF;
}

std::string RTC::getDateTime() {
    return "not completed";
}

int RTC::setRegister(Register reg, uint8_t byte) {
    int write = wiringPiI2CWriteReg8(this->fd, reg, byte);
    i2c_status = write;
    return write;
}

int RTC::setBattery(bool state) {
    uint8_t value = this->getRegister(WEEKDAY);
    if (state) {
        value |= 0b1000;
    } else {
        value &= 0b11110111;
    }

    int write = this->setRegister(WEEKDAY, value);
    return write;
}

int RTC::setClock(bool state) {
    uint8_t value = this->getRegister(SEC);
    
    if (state) {
        value |= 0b10000000;
    } else {
        value &= 0b01111111;
    }
    
    int write = this->setRegister(SEC, value); 
    this->checkTick();
    return write;
}

int RTC::setSeconds(int value) {
    if (verifyDate(SEC, value)) {
        uint8_t enc = RTC::encodeDecimal(value);
        uint8_t byte = (this->clock << 7) | enc;
        return this->setRegister(SEC, byte);
    }
    return -1;
}

int RTC::setMinutes(int value) {
    if (verifyDate(MIN, value)) {
        uint8_t enc = RTC::encodeDecimal(value);
        return this->setRegister(MIN, enc);
    }
    return -1;
}

int RTC::setHours(int value) {
    if (verifyDate(HOUR, value)) {
        uint8_t raw = this->getRegister(HOUR);
        uint8_t enc = RTC::encodeDecimal(value);
        uint8_t byte = (raw & 0b01000000) | enc;
        return this->setRegister(HOUR, enc);
    }
    return -1;
}

int RTC::setWeekDay(int value) {
    if (verifyDate(WEEKDAY, value)) {
        uint8_t raw = this->getRegister(WEEKDAY);
        uint8_t enc = RTC::encodeDecimal(value);
        uint8_t byte = (raw & 0b00100000) | enc;
        return this->setRegister(WEEKDAY, enc);
    }
    return -1;
}

int RTC::setDate(int value) {
    if (verifyDate(DATE, value)) {
        uint8_t enc = RTC::encodeDecimal(value);
        return this->setRegister(WEEKDAY, enc);
    }
    return -1;
}

int RTC::setMonth(int value) {
    if (verifyDate(MONTH, value)) {
        uint8_t raw = this->getRegister(MONTH);
        uint8_t enc = RTC::encodeDecimal(value);
        uint8_t byte = (raw & 0b11100000) | enc;
        return this->setRegister(MONTH, enc);
    }
    return -1;
}

int RTC::setYear(int value) {
    if (verifyDate(YEAR, value)) {
        uint8_t enc = RTC::encodeDecimal(value - 2000);
        return this->setRegister(YEAR, enc);
    }
    return -1;
}

int RTC::setDateTime(std::string time) {
    struct tm tm;
    if(strptime(time.c_str(), "%Y-%m-%d %H:%M:%S", &tm)){
        this->setSeconds(tm.tm_sec);
        this->setMinutes(tm.tm_min);
        this->setHours(tm.tm_hour);
        this->setDate(tm.tm_mday);
        this->setMonth(tm.tm_mon);
        this->setYear(tm.tm_year);
        this->i2c_status = 0;
    }
    this->i2c_status = 1;
    return -1;
}

uint8_t RTC::encodeDecimal(int value) {
    uint8_t ones = value % 10;
    uint8_t tens = uint8_t(value / 10);
    return (tens << 4) + ones;
}

bool RTC::verifyDate(Register reg, int value) {

    return true;
}
