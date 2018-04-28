/******************************************************************************
Data collection and initial processing

Polls the SPI slave for new data. The data comes in as raw "blocks" containing
a timestamp and a set of pin values. As these raw blocks are collected, they
are turned into "ping" structs that can be consumed by the higher-level
functions that will determine location and UID.

Author:  Noah Strong
Project: Crab Tracker
Created: 2018-02-10
******************************************************************************/
#include "spi.h"
#include "config.h"

#ifndef __DATA_COLLECTION_H
#define __DATA_COLLECTION_H

int proc_block(spi_rawblock, ping*);
void disp_ping(ping);
int poll(ping*);

#endif /* __DATA_COLLECTION_H */
