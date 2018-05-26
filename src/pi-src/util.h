/******************************************************************************
Helper methods

Author:  Noah Strong
Project: Crab Tracker
Created: 2018-02-19
******************************************************************************/

#ifndef __UTIL_H
#define __UTIL_H
#include <stdint.h>

int initialize_util();
int get_param(char*, int*);
void print_bin_8(uint8_t);
void print_bin_ulong(unsigned long);

#endif // __UTIL_H
