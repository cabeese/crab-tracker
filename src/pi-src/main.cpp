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
using namespace std;

struct spi_rawblock {
    unsigned long timestamp;
    uint8_t pinvals;
};

/**********************************************************
Declare Global Variables
***********************************************************/
int fd;
unsigned char result;

/**********************************************************
Declare Functions
***********************************************************/
int spi_getbyte();
int spi_getblock(spi_rawblock*);
void spi_dispblock(spi_rawblock);


/**********************************************************
Main
  Setup SPI
	Open file spidev0.0 (chip enable 0) for read/write
	  access with the file descriptor "fd"
	Configure transfer speed (1MkHz)
  Start an endless loop that repeatedly sends the characters
	in the hello[] array to the Ardiuno and displays
	the returned bytes
***********************************************************/
int main (void) {
    fd = open("/dev/spidev0.0", O_RDWR);
    spi_rawblock block;
    /*
    We need to send an 8-bit state/bitmask, then a 32-bit long timestamp
     */

    unsigned int speed = 1000000;
    ioctl (fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);

    /* For some reason, we get some junk data on the first byte we grab.
        TODO: find out why this happens! Could by SPI thing or could just be
          some dumb thing in the Arduino code I did.
    */
    spi_getbyte(); /* Throw away the first byte */

    while (1){
        cout << "Enter a char to get more data via SPI. ";
        getchar();

        /* Get and display a block of data from the pi */
        result = spi_getblock(&block);
        spi_dispblock(block);
        // result = spi_getbyte();
        // printf("%d\n", result);
        // cout << result;
        // usleep (10); /* Why 10?? */
    }
}

/**
 * Grabs one byte via SPI.
 *
 * Establishes a data structure, spi_ioc_transfer as defined
 * by spidev.h and loads the various members to pass the data
 * and configuration parameters to the SPI device via IOCTL
 *
 * Returns a single byte (as in int)
 */
int spi_getbyte(){
    /* Unused, but we have to send SOMETHING. It's the [SPI] rule */
    unsigned char txDat = ' ';
    unsigned char rxDat;
    struct spi_ioc_transfer spi;

    memset (&spi, 0, sizeof (spi));

    spi.tx_buf        = (unsigned long)&txDat;
    spi.rx_buf        = (unsigned long)&rxDat;
    spi.len           = 1;

    ioctl (fd, SPI_IOC_MESSAGE(1), &spi);

    printf(" [raw=0x%x]\n", rxDat);
    return rxDat;
}

int spi_getblock(spi_rawblock *data){
    uint8_t pinvals;
    unsigned long timestamp = 0;

    /* Get pinvals */
    pinvals = spi_getbyte();

    /* Get timestamp (in 4 parts) */
    for(int i=0; i<4; i++){
        timestamp |= spi_getbyte() << (i * 8);
        // printf("  timestamp now: 0x%x\n", timestamp);
    }

    data->pinvals = pinvals;
    data->timestamp = timestamp;
    return 1;
}

void spi_dispblock(spi_rawblock data){
    printf("--- SPI Raw Data Block ---\n");
    printf("pinvals: 0x%x\n", data.pinvals);
    printf("timestamp: 0x%x\n", data.timestamp);
}
