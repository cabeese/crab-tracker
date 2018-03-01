/******************************************************************************
Finds the direciton.

Author:  Lizzy Schoen
Project: Crab Tracker
Created: 2018-03-1
******************************************************************************/

#ifndef __DIRECTION_H
#define __DIRECTION_H

typedef struct {
  double x;
  double y;
  double z;
} xyz;

int triangulation( /* 'ping' arguments TBD, */ xyz *result);

double calcN(long delta_1, long delta_2, long delta_3);
double calcX(long delta_2, double N);
double calcY(long delta_1, double N);
double calcZ(double N, double x, double y);


#endif /* __DIRECTION_H */
