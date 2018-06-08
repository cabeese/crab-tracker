/******************************************************************************
Facilitates the transfer of data from the Arduino to the Pi via SPI.

Author:  Noah Strong
Project: Crab Tracker
Created: 2018-02-10
******************************************************************************/
#include <stdint.h>

#ifndef __SPI_H
#define __SPI_H

/* The speed (in MHz) of the timestamp collector device (e.g. Arduino Nano) */
#define TIMESTAMP_COLLECTOR_CLOCKSPEED_MHz 16
#define CYCLE_LENGTH_US 1.0/TIMESTAMP_COLLECTOR_CLOCKSPEED_MHz

#define SPI_NO_FLAGS 0xF     /* Default behavior */
#define SPI_RESET 0x1        /* Reset counters */
#define SPI_ECHO_REQUEST 0x2 /* Used for connection test */
#define SPI_ECHO_EXPECTED_RESPONSE 0x77 /* Expected back in connection test */

struct spi_rawblock {
    double timestamp_us;
    uint8_t pinvals;
};

uint8_t spi_getbyte(uint8_t);
int spi_getblock(spi_rawblock*);
void spi_dispblock(spi_rawblock);
int spi_echo_test();
void initialize_spi();

#endif /* __SPI_H */
