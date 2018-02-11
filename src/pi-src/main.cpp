/******************************************************************************
Project: Crab Tracker

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


/**********************************************************
Declare Global Variables
***********************************************************/
int spifd;
unsigned char result;

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
    spifd = open("/dev/spidev0.0", O_RDWR);
    spi_rawblock block;
    /*
    We need to send an 8-bit state/bitmask, then a 32-bit long timestamp
     */

    unsigned int speed = 1000000;
    ioctl (spifd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);

    /* For some reason, we get some junk data on the first byte we grab.
        TODO: find out why this happens! Could by SPI thing or could just be
          some dumb thing in the Arduino code I did.
    */
    spi_getbyte(spifd); /* Throw away the first byte. Why is this? */

    while (1){
        cout << "Enter a char to get more data via SPI. ";
        getchar();

        /* Get and display a block of data from the pi */
        result = spi_getblock(spifd, &block);
        spi_dispblock(block);
        // result = spi_getbyte();
        // printf("%d\n", result);
        // cout << result;
        // usleep (10); /* Why 10?? */
    }
}
