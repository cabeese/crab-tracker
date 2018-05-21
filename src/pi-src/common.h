/******************************************************************************
Shared prototypes, structs, global variables, etc.

Author:  Noah Strong
Project: Crab Tracker
Created: 2018-04-28
******************************************************************************/

#ifndef __COMMON_H
#define __COMMON_H

struct ping {
    int pin;
    unsigned long start;
    unsigned long duration;
};

struct crab_event {
    double r_m;   /* Distance in meters */
    double theta; /* Offset from due north (units?) */
    double z_m;   /* Depth in meters */
    int id;       /* Unique ID of the crab (-1 if uncalculated) */
    int inert;    /* Non-zero if crab is inert */
    double N;        /* For debugging purposes only */
};

#endif /* __COMMON_H */
