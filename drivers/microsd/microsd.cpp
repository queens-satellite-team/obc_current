#include "microsd.h"
#include "spidevice.h"
#include <iostream>

MicroSD::~MicroSD(){
    this->spi.releaseSpiFd();
}

MicroSD::MicroSD(){
    this->spi = {
        .busNum = 1,
        .deviceId = 1,
        .speed_hz = 1320000,
        .delay_usecs = 0,
        .bits_per_word = 32,
        .cs_change = 0,
        .word_delay_usecs = 0,
    };
    this->status = this->spi.setupSpiDevice();
}

uint8_t* MicroSD::read(uint8_t addr) {
    int size = 2;
    uint8_t* tx = (uint8_t*)malloc(sizeof(uint8_t) * size);
    uint8_t* rx = (uint8_t*)malloc(sizeof(uint8_t) * size);
    tx[0] = 0;
    tx[1] = 0;
    spi.spiTransfer(tx, rx, size);
    
    free(tx);
    free(rx);
    return rx;
}

int MicroSD::write(uint8_t addr, uint8_t *buf, int bytes) {
    int size = bytes;
    uint8_t* tx = (uint8_t*)malloc(sizeof(uint8_t) * (size + 1));
    uint8_t* rx = (uint8_t*)malloc(sizeof(uint8_t) * (size + 1));
    tx[0] = addr;
    spi.spiTransfer(tx, rx, size);
    free(tx);
    free(rx);
    return -1;
}
