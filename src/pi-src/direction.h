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

typedef struct {
  double x;
  double y;
  double z;
  double N;
  double r;
  double theta;
} data;

int triangulation( /* 'ping' arguments TBD, */ data *result);
int triangulation_helper(unsigned long ts_a, unsigned long ts_b, unsigned long ts_c, unsigned long ts_d, data *result);

/*
 * Helper functions calculate various calculations
*/
void printResult(data *result);
double calcN(double delta_1, double delta_2, double delta_3);
double calcX(double delta_1, double N);
double calcY(double delta_3, double N);
double calcZ(double N, double x, double y);
double calcR(double x, double y);
double calcAngle(double x, double y);


#endif /* __DIRECTION_H */
