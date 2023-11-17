#ifndef SPI_DEVICE_H
#define SPI_DEVICE_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#define SPI_DEVICE_PATH "/dev/spidev%d-%d"

int initSpiFd(int *fd, int busNum, int deviceID);
int setupSpiDevice();
int releaseSpiFd(int fd);
int spiTransfer(int fd, uint8_t *tx, uint8_t *rx, int bytes);

#endif


