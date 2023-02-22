#ifndef SPI_H
#define SPI_H

#include <stdint.h>

int SPI_init();

uint8_t transferData(uint8_t sendData);

void closeSPI();

#endif



