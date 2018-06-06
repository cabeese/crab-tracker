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
#include "common.h"

#ifndef __DATA_COLLECTION_H
#define __DATA_COLLECTION_H

struct full_set {
    ping *pings_a[2];
    ping *pings_b[2];
    ping *pings_c[2];
    ping *pings_d[2];
};

int initialize_dc();
int proc_block(spi_rawblock);
void disp_ping(ping);
void disp_buffers();
int poll();
int get_set(full_set*);
void clear_set(full_set*);

#endif /* __DATA_COLLECTION_H */
