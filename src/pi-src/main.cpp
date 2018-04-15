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
#include "util.h"
#include "config.h"
#include "spi.h"
#include "data_collection.h"

int DISPLAY_RAW_SPI;
unsigned int result;
spi_rawblock RAW = {0, 0};
ping storage[5]; /* Eventual storage for pings that come in. Currently unused */

/**
 * Initialize all 'modules' in the program.
 * @return (currently unused)
 */
int initialize(){
    initialize_util();
    initialize_spi();
    initialize_dc();
    get_param((char*)"DISPLAY_RAW_SPI", &DISPLAY_RAW_SPI);
    return 1;
}

/**
 * Initialize settings and start listening for and processing data.
 * @return  (unused)
 */
int main (void) {
    initialize();

    for(int i=0; i<5; i++){ storage[i] = {0, 0}; }

    while (1){
        spi_getblock(&RAW);
        if(DISPLAY_RAW_SPI) spi_dispblock(RAW);
        result = proc_block(RAW, &(*storage));
        if(result){
            // no-op for now
        } else {
            sleep(1);
        }
        usleep(100);
    }
}
