/**
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
#define r 1 /*  "r"  the length of the hydrophone square          */
const double S_USER = 1.5/1000;

typedef struct {
  double dist_d;
  double dist_a;
  double dist_c;
  double dist_b;
} abcd;

typedef struct {
  long delta_1;
  long delta_2;
  long delta_3;
} deltas;

typedef struct {
  long ts_a;
  long ts_b;
  long ts_c;
  long ts_d;
} timestamps;

int dist(abcd *result);

int calcDist(long x, long y, long z, abcd *result);
double calcDeltas(long delta_2, double N);
double calcTime(long delta_1, double N);
//void printResult(xyz *result);

int main(int argc, const char* argv[]) {
  abcd result;
  dist(&result);

  // deltas r2;
  // calcDeltas(result, deltas *r2);
  //
  // timestamps r3;
  // calcTime(result, timestamps *r3);

  return 1;
}

int dist(abcd *result){
  long x = 30;
  long y = -19;
  long z = -70;
  calcDist(x, y, z, result);
  return 1;
}

int calcDist(long x, long y, long z, abcd *result){
  //Calculate distance
  double a = sqrt(pow((x - (r/2.0)),2.0) + pow((y - (r/2.0)),2.0) + pow(z, 2.0));
  fprintf(stderr, "dist A %f\n", a);
  double b = sqrt(pow((x + (r/2.0)),2.0) + pow((y - (r/2.0)),2.0) + pow(z, 2.0));
  fprintf(stderr, "dist b %f\n", b);
  double c = sqrt(pow((x + (r/2.0)),2.0) + pow((y + (r/2.0)),2.0) + pow(z, 2.0));
  fprintf(stderr, "dist c %f\n", c);
  double d = sqrt(pow((x - (r/2.0)),2.0) + pow((y + (r/2.0)),2.0) + pow(z, 2.0));
  fprintf(stderr, "dist d %f\n", d);

  //Time stamps
  long ts_a = a / .0015;
  fprintf(stderr, "ts_a %ld\n", ts_a);
  long ts_b = b / .0015;
  fprintf(stderr, "ts_b %ld\n", ts_b);
  long ts_c = c / .0015;
  fprintf(stderr, "ts_c %ld\n", ts_c);
  long ts_d = d / .0015;
  fprintf(stderr, "ts_d %ld\n", ts_d);

  //deltas
  long d1 = ts_a - ts_d;
  fprintf(stderr, "Delta 1 %ld\n", d1);
  long d2 = ts_c - ts_d;
  fprintf(stderr, "Delta 2 %ld\n", d2);
  long d3 = ts_b - ts_d;
  fprintf(stderr, "Delta 3 %ld\n", d3);


  result->dist_a = a;
  result->dist_b = b;
  result->dist_c = d;
  result->dist_d = d;

  return 1;
}
