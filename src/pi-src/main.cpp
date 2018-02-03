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


/**********************************************************
Declare Global Variables
***********************************************************/

int fd;
unsigned char result;

/**********************************************************
Declare Functions
***********************************************************/

int spi_getbyte();


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
    char buf[1000];
    /*
    We need to send an 8-bit state/bitmask, then a 32-bit long timestamp
     */

    unsigned int speed = 1000000;
    ioctl (fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);

    while (1){
        result = spi_getbyte();
        printf("%d\n", result);
        // cout << result;
        // usleep (10); /* Why 10?? */
        cout << "\nEnter a char to continue\n";
        getchar();
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

    return rxDat;
}
