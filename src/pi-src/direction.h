/******************************************************************************
Finds the direciton of a crab using Time Difference On Arrival (TDOA).

Author:  Lizzy Schoen
Project: Crab Tracker
Created: 2018-03-1
******************************************************************************/

#include "common.h"

#ifndef __DIRECTION_H
#define __DIRECTION_H

double calcSpeedOfSound(double temp, double salinity, double depth);

int triangulation(struct ping a, struct ping b, struct ping c, struct ping d, crab_event *result);
int triangulation_helper(double ts_a, double ts_b, double ts_c, double ts_d, crab_event *result);

/*
 * Helper functions calculate various calculations
*/
void printResult(crab_event *result);
void disp_direction(crab_event);
double calcN(double delta_1, double delta_2, double delta_3);
double calcX(double delta_1, double N);
double calcY(double delta_3, double N);
double calcZ(double N, double x, double y);
double calcR(double x, double y);
double calcAngle(double x, double y);
int initialize_dir();

#endif /* __DIRECTION_H */
