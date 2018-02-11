/**********************************************************
Data collection and initial processing

Polls the SPI slave for new data. The data comes in as raw "blocks" containing
a timestamp and a set of pin values. As these raw blocks are collected, they
are turned into "ping" structs that can be consumed by the higher-level
functions that will determine location and UID.

Author:  Noah Strong
Project: Crab Tracker
Created: 2018-02-10
***********************************************************/

struct ping {
    int pin;
    unsigned long start;
    unsigned long duration;
};
