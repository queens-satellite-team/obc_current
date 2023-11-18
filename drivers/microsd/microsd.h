#ifndef MICROSD_H
#define MICROSD_H

#include <unistd.h>
#include <iostream>
#include <cstdio>

#include <cstdint>
#include <unistd.h>
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <bitset>

#include "spidevice.h"

class MicroSD {

public:
    ~MicroSD();
    MicroSD();
    void readBlocks(uint32_t blockNum, uint8_t* buf);
    void writeBlocks(uint32_t blockNum, const uint8_t* buf);
    uint8_t* read(uint8_t addr);
    int write(uint8_t addr, uint8_t* buf, int bytes);

private:
    spi_device spi;
    int status; // only lets you write or read if the status is 1 (device is setup properly)

    int initSPI();
};

#endif
