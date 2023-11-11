#include "microsd.h"
#include <iostream>
#include <linux/spi/spidev.h>
#include <unistd.h>

MicroSD::~MicroSD(){
    close(fd);
}

MicroSD::MicroSD(){

}

MicroSD::MicroSD(const char* device, int cs, int baudrate)
    : cs(cs) {



}

void MicroSD::initSPI(const char* device, int baudrate){
    this->fd = open(device, O_RDWR);
    if (fd < 0) {
        std::cerr << "Can't open device" << "\n";
    }

    uint8_t mode = SPI_MODE_0;

    int ret = ioctl(this->fd, SPI_IOC_WR_MODE, &mode);
    if (ret == -1) {
        std::cerr << "Can't set spi mode" << "\n";
    }

	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &this->spi.bits_per_word);
	if (ret == -1) {
		std::cerr << "can't set max speed hz" << "\n";
    }

    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &this->spi.speed_hz);
	if (ret == -1) {
		std::cerr << "can't set max speed hz" << "\n";
    }
    
}
