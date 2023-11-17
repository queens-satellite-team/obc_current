#include "spidevice.h"

int initSpiFd(int *fd, int busNum, int deviceID){
    char pathname[256];

    int status = snprintf(pathname, sizeof(pathname), SPI_DEVICE_PATH, deviceID, busNum);
    
    //check status to be good
    if (status < 0) {
		perror("Invalid Bus Number and/or Device ID");
        return EXIT_FAILURE;
    }

    *fd = open(pathname, O_RDWR);

    if (*fd < 0) {
		perror("Could not open SPI file");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int releaseSpiFd(int fd){
    return close(fd);
}

int setupSpiDevice() {
    int fd, status;
    
    status = initSpiFd(&fd, 0, 0);

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

int spiTransfer(int fd, uint8_t *tx, uint8_t *rx, int bytes){
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

int doSPI(){
    const char* name = "something";
    struct spi_ioc_transfer xfer[2];
    unsigned char buf[32], *bp;
    int len, status;

    int fd = open(name, O_RDWR);
    if (fd < 0) {
        perror("Could not open SPI file");
        return -1;
    }

    memset(xfer, 0, sizeof xfer);
    memset(buf, 0, sizeof buf);
    len = sizeof buf;

    buf[0] = 0x00; // make first element of the buffer the command
    
    xfer[0].tx_buf = (unsigned long) buf;
    xfer[0].len = 1;

    xfer[1].rx_buf = (unsigned long) buf;
    xfer[1].len = 6;

    status = ioctl(fd, SPI_IOC_MESSAGE(2), xfer);
    if(status < 0){
        perror("Could not send SPI message");
        return -1;
    }

    printf("%s\n", buf);

    return 0;
}   

