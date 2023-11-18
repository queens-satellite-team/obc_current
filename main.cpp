//
// Created by hanat on 2023-10-28.
//

#include <iostream>

#include "drivers/microsd/microsd.h"

void doMicrosd() {
    MicroSD *sd = new MicroSD();
    while(true){
        uint8_t* rx = sd->read(0);
        printf("Read info %02X\n", rx[0]);
        free(rx);
    }
}

int main() { 
    std::cout << "Hello world" << "\n";
    doMicrosd();
    return 0;
};
