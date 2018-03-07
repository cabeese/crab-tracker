/*
 *  Triangulation Algorithm Test
 *  Chloe Yugawa February 2018
 *  February 2018
 *
 */
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <vector>
#include <stdio.h>
//#include "test.h"
#define R 1 /*  "r"  the length of the hydrophone square          */
const double S_USER = 1.5/1000;

typedef struct {
  double x;
  double y;
  double z;
  double N;
  double r;
  double theta;
} data;

typedef struct {
  double ts_a;
  double ts_b;
  double ts_c;
  double ts_d;
} timestamps;

int calcDist(double x, double y, double z, data *result, timestamps *times);
void printResult(data *result);
int main(int argc, const char* argv[]) {
  data result;
  timestamps times;
  double x = -10.0;
  double y = -15.0;
  double z = -30.0;
  calcDist(x, y, z, &result, &times);
  printResult(&result);
  return 1;
}

int calcDist(double x, double y, double z, data *result, timestamps *times){
  result->x = x;
  result->y = y;
  result->z = z;
  //Calculate distance
  //Line below is N
  double a = sqrt(pow((x - R),2.0) + pow((y - R),2.0) + pow(z, 2.0));
  double b = sqrt(pow((x + R),2.0) + pow((y - R),2.0) + pow(z, 2.0));
  double c = sqrt(pow((x + R),2.0) + pow((y + R),2.0) + pow(z, 2.0));
  double d = sqrt(pow((x - R),2.0) + pow((y + R),2.0) + pow(z, 2.0));
  result->N = a;

  //Time stamps
  double ts_a = a / .0015;
  double ts_b = b / .0015;
  double ts_c = c / .0015;
  double ts_d = d / .0015;
  times->ts_a = ts_a;
  times->ts_b = ts_b;
  times->ts_c = ts_c;
  times->ts_d = ts_d;

  //deltas
  double d1 = ts_b - ts_a;
  double d2 = ts_c - ts_a;
  double d3 = ts_d - ts_a;

  //r and theta_degrees
  double radius = (double)sqrt(pow(x,2.0) + pow(y,2.0));
  double pi = (double)atan(1)*4;
  double theta_degrees = (double)atan2(y,x) * 180 / pi;
  result->r = radius;
  result->theta = theta_degrees;

  return 1;
}
void printResult(data *result){
  fprintf(stderr, "x = %lf\n", result->x);
  fprintf(stderr, "y = %lf\n", result->y);
  fprintf(stderr, "z = %lf\n", result->z);
  fprintf(stderr, "N = %lf\n", result->N);
  fprintf(stderr, "r = %lf\n", result->r);
  fprintf(stderr, "theta = %lf\n", result->theta);
}
