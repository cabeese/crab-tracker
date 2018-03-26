/******************************************************************************
Helper methods

Author:  Noah Strong
Project: Crab Tracker
Created: 2018-02-19
******************************************************************************/

#include <stdint.h>
#include <stdio.h>

/**
 * Print a 32-bit number as binary.
 * Written by Zach McGrew (WWU Comptuer Science Graduate Student)
 * @param number The number to print
 */
void print_bin(int32_t number) {
  uint32_t bit_set = (uint32_t) 1 << 31;
  int32_t bit_pos;

  for (bit_pos = 31; bit_pos >= 0; --bit_pos, bit_set >>= 1) {
    printf("%c", ((uint32_t)number & bit_set ? '1' : '0'));
  }
}

/**
 * Print an 8-bit unsigned integer in binary to stdout.
 * @param number The number (UNSIGNED) to print
 */
void print_bin_8(uint8_t number){
    uint8_t bit_set = (uint8_t) 1 << 7;
    int bit_pos;

    for(bit_pos = 7; bit_pos >= 0; --bit_pos){
        printf("%c", ((uint8_t)number & bit_set ? '1' : '0'));
        bit_set >>= 1;
    }
}

/**
 * Print a long unsigned integer in binary to stdout.
 * @param number The number (UNSIGNED) to print
 */
void print_bin_ulong(unsigned long number){
    unsigned long bit_set = (unsigned long) 1 << 31;
    int bit_pos;

    for(bit_pos = 31; bit_pos >= 0; --bit_pos){
        printf("%c", (number & bit_set ? '1' : '0'));
        bit_set >>= 1;
    }
}
