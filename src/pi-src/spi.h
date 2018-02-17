/******************************************************************************
Facilitates the transfer of data from the Arduino to the Pi via SPI.

Author:  Noah Strong
Project: Crab Tracker
Created: 2018-02-10
******************************************************************************/
#include <stdint.h>

#ifndef __SPI_H
#define __SPI_H

#define SPI_NO_FLAGS 0x0     /* Default behavior */
#define SPI_RESET 0x1        /* Reset counters */
#define SPI_ECHO_REQUEST 0x2 /* Send specific message. TODO: implement */

#define SPI_ECHO_EXPECTED_RESPONSE 0x77 /* Expected back from Arduino. */

struct spi_rawblock {
    unsigned long timestamp;
    uint8_t pinvals;
};

uint8_t spi_getbyte(uint8_t);
int spi_getblock(spi_rawblock*);
void spi_dispblock(spi_rawblock);
int spi_echo_test();
void initialize_spi();

#endif /* __SPI_H */
