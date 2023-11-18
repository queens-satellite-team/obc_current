#include "spidevice.h"

int spi_device::initSpiFd(){
    char pathname[256];

    int status = snprintf(pathname, sizeof(pathname), SPI_DEVICE_PATH, deviceId, busNum);
    printf("Attempting to setup SPI for %s\n", pathname);

    //check status to be good
    if (status < 0) {
		perror("Invalid Bus Number and/or Device ID");
        return EXIT_FAILURE;
    }

    fd = open(pathname, O_RDWR);

    if (fd < 0) {
		perror("Could not open SPI file");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int spi_device::releaseSpiFd(){
    return close(fd);
}

int spi_device::setupSpiDevice() {
    int status;
    
    status = this->initSpiFd();

    if (status == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    /*params*/
    int mode, bits_per_word, speed_hz;

    int ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (ret == -1) {
        perror("Can't set spi mode");
        return EXIT_FAILURE;
    }

	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word);
	if (ret == -1) {
		perror("can't set bits per word");
        return EXIT_FAILURE;
    }

    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed_hz);
	if (ret == -1) {
		perror("can't set max speed hz");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int spi_device::spiTransfer(uint8_t *tx, uint8_t *rx, int bytes){
    int status, res;
    struct spi_ioc_transfer xfer;
    
    res = EXIT_FAILURE;
    
    memset(&xfer, 0, sizeof xfer);
    xfer.tx_buf = (unsigned long)tx;
    xfer.rx_buf = (unsigned long)rx;
    xfer.len = bytes;
    /*MIGHT HAVE TO set other xfer attributes*/
    
    status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);

    if (status < 0) {
        perror("Could not transfer to device");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
