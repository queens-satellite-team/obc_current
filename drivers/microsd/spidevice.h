#ifndef SPI_DEVICE_H
#define SPI_DEVICE_H

#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define SPI_DEVICE_PATH "/dev/spidev%d-%d"

struct spi_device {
    int fd;
    int busNum;
    int deviceId;

    uint32_t speed_hz;

    uint16_t delay_usecs;

    uint8_t bits_per_word;
    uint8_t cs_change;
    uint8_t word_delay_usecs;

    int initSpiFd();
    int setupSpiDevice();
    int releaseSpiFd();
    int spiTransfer(uint8_t *tx, uint8_t *rx, int bytes);
};

#endif
