/******************************************************************************
Facilitates the transfer of data from the Arduino to the Pi via SPI.

Author:  Noah Strong
Project: Crab Tracker
Created: 2018-02-10
******************************************************************************/
#include <stdint.h>

#ifndef __SPI_H
#define __SPI_H

#define SPI_RESET 0x1
#define SPI_ECHO_REQUEST 0x2 /* TODO: implement */

struct spi_rawblock {
    unsigned long timestamp;
    uint8_t pinvals;
};

uint8_t spi_getbyte();
int spi_getblock(spi_rawblock*);
void spi_dispblock(spi_rawblock);
void initialize_spi();

#endif /* __SPI_H */
