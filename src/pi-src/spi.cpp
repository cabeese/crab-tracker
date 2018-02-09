/**********************************************************
 SPI_Hello_Arduino
   Configures an Raspberry Pi as an SPI master and
   demonstrates bidirectional communication with an
   Arduino Slave by repeatedly sending the text
   "Hello Arduino" and receiving a response

Code taken from http://robotics.hobbizine.com/raspiduino.html
***********************************************************/

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include "spi.h"
using namespace std;

int fd;

/**
 * Grabs one byte via SPI.
 *
 * Establishes a data structure, spi_ioc_transfer as defined
 * by spidev.h and loads the various members to pass the data
 * and configuration parameters to the SPI device via IOCTL
 *
 * Returns a single byte (as in int)
 */
int spi_getbyte(int spifd){
    /* Unused, but we have to send SOMETHING. It's the [SPI] rule */
    unsigned char txDat = ' ';
    unsigned char rxDat;
    struct spi_ioc_transfer spi;

    memset (&spi, 0, sizeof (spi));

    spi.tx_buf        = (unsigned long)&txDat;
    spi.rx_buf        = (unsigned long)&rxDat;
    spi.len           = 1;

    ioctl (spifd, SPI_IOC_MESSAGE(1), &spi);

    printf(" [raw=0x%x]\n", rxDat);
    return rxDat;
}

int spi_getblock(int spifd, spi_rawblock *data){
    uint8_t pinvals;
    unsigned long timestamp = 0;

    /* Get pinvals */
    pinvals = spi_getbyte(spifd);

    /* Get timestamp (in 4 parts) */
    for(int i=0; i<4; i++){
        timestamp |= spi_getbyte(spifd) << (i * 8);
        // printf("  timestamp now: 0x%x\n", timestamp);
    }

    data->pinvals = pinvals;
    data->timestamp = timestamp;
    return 1;
}

void spi_dispblock(spi_rawblock data){
    printf("--- SPI Raw Data Block ---\n");
    printf("pinvals: 0x%x\n", data.pinvals);
    printf("timestamp: 0x%lx\n", data.timestamp);
}
