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
unsigned char result;

/**
 * Initialize settings and start listening for and processing data.
 * @return  (unused)
 */
int main (void) {
    spi_rawblock block;

    initialize_spi();

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
