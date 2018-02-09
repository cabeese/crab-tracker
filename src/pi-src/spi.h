/**
 * SPI Header
 */
#include <stdint.h>

struct spi_rawblock {
    unsigned long timestamp;
    uint8_t pinvals;
};

int spi_getbyte(int spifd);
int spi_getblock(int spifd, spi_rawblock*);
void spi_dispblock(spi_rawblock);
