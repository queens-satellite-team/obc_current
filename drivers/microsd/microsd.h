#ifndef MICROSD_H
#define MICROSD_H

#include <cstdint>
#include <unistd.h>
#include <ctime>
#include <iostream>
#include <stdio.h>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <bitset>

#define MOSI

class MicroSD {

public:
    ~MicroSD();
    MicroSD();
    MicroSD(const char* device, int cs, int baudrate = 1320000);
    void readBlocks(uint32_t blockNum, uint8_t* buf);
    void writeBlocks(uint32_t blockNum, const uint8_t* buf);

private:
    int fd;
    int cs;
    uint8_t cmdbuf[6];
    uint8_t dummybuf[512];
    uint8_t tokenbuf[1];
    uint32_t sectors;
    int cdv;

    void initSPI(const char* device, int baudrate);
    void initCard(int baudrate);
    uint8_t cmd(uint8_t cmd, uint32_t arg, uint8_t crc, int final = 0, bool release = true, bool skip1 = false);
    void readinto(uint8_t* buf);
    void write(uint8_t token, const uint8_t* buf);
    void writeToken(uint8_t token);
};

#endif
