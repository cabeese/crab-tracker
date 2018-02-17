/******************************************************************************
Facilitates the transfer of data from the Arduino to the Pi via SPI.

Code based on http://robotics.hobbizine.com/raspiduino.html

Author:  Noah Strong
Project: Crab Tracker
Created: 2018-02-10
******************************************************************************/

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include "spi.h"
using namespace std;

int spifd;
int _INITIALIZED = 0;

/**
 * Grabs one byte via SPI.
 *
 * Establishes a data structure, spi_ioc_transfer as defined
 * by spidev.h and loads the various members to pass the data
 * and configuration parameters to the SPI device via IOCTL
 *
 * @param flags - The flag(s) to send to the slave. Flags are defined in spi.h
 * Returns a single byte (as in int)
 */
uint8_t spi_getbyte(uint8_t flags){
    uint8_t txDat = flags;
    uint8_t rxDat;
    struct spi_ioc_transfer spi;

    /* From /usr/include/linux/spi/spidev.h on the Pi:
    * "Zero-initialize the structure, including currently unused fields, to
    * accommodate potential future updates."
    */
    memset (&spi, 0, sizeof (spi));

    spi.tx_buf        = (unsigned long)&txDat;
    spi.rx_buf        = (unsigned long)&rxDat;
    spi.len           = 1;

    ioctl (spifd, SPI_IOC_MESSAGE(1), &spi);

    printf(" [raw=0x%x] (%ld bytes)\n", rxDat, sizeof(rxDat));
    return rxDat;
}

/**
 * Grabs a full "block" (5 8-byte transmissions) from SPI and stores them in
 *     a struct.
 * @param  data - Out parameter. Incoming data is stored here.
 * @return      - 1 (unused currently)
 */
int spi_getblock(spi_rawblock *data){
    uint8_t pinvals;
    unsigned long timestamp = 0;

    /* Get pinvals */
    pinvals = spi_getbyte(SPI_NO_FLAGS);

    /* Get timestamp (in 4 parts) */
    for(int i=0; i<4; i++){
        timestamp |= spi_getbyte(SPI_NO_FLAGS) << (i * 8);
    }

    data->pinvals = pinvals;
    data->timestamp = timestamp;
    return 1;
}

/**
 * Print an SPI 'raw block' struct to stdout.
 * @param data - The block to print.
 */
void spi_dispblock(spi_rawblock data){
    printf("--- SPI Raw Data Block ---\n");
    printf("pinvals: 0x%x\n", data.pinvals);
    printf("timestamp: 0x%lx\n", data.timestamp);
}

/**
 * Sends a quick test to ensure that the SPI connection between the two devices
 *     is working as intended.
 * Requests that the next byte is the "RESPONSE" pattern. It then fetches that
 *     byte and resets the counters on the Arduino so data can still be read.
 * @return 1 if connection is working; 0 otherwise
 */
int spi_echo_test(){
    spi_getbyte(SPI_ECHO_REQUEST); /* Next byte should be response */
    return spi_getbyte(SPI_RESET) == SPI_ECHO_EXPECTED_RESPONSE;
}

/**
 * Open up the SPI file (chip enable 0) and configure speed.
 * This function must be called once before SPI is used.
 */
void initialize_spi(){
    if(!_INITIALIZED){
        /*
          Open file spidev0.0 (chip enable 0) for read/write
          Configure transfer speed (1MkHz)
        */
        unsigned int speed = 1000000;
        spifd = open("/dev/spidev0.0", O_RDWR);
        ioctl (spifd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);

        /* For some reason, we get some junk data on the first byte we grab.
            TODO: find out why this happens! Could by SPI thing or could just be
              some dumb thing in the Arduino code I did. */
        spi_getbyte(SPI_RESET); /* Throw away the first byte. Why is this? */

        if(spi_echo_test()){
            std::cout << "SPI test PASSED" << '\n';
        } else {
            std::cout << "SPI test FAILED!!!!" << '\n';
        }

        _INITIALIZED = 1;
    }
}
