/******************************************************************************
Facilitates the transfer of data from the Arduino to the Pi via SPI.

Author:  Noah Strong
Project: Crab Tracker
Created: 2018-02-10
******************************************************************************/
#include <stdint.h>

#define SPI_RESET 0x1

struct spi_rawblock {
    unsigned long timestamp;
    uint8_t pinvals;
};

uint8_t spi_getbyte(int spifd);
int spi_getblock(int spifd, spi_rawblock*);
void spi_dispblock(spi_rawblock);
