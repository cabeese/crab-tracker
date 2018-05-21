/**
 *  Triangulation Algorithm Implementation
 *  Elizabeth Schoen February 2018
 *  February 2018
 *
 */
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <iostream>
#include "common.h"
#include "direction.h"
#define R_USER 1 /*  "r"  half the length of the hydrophone square          */
const double S_USER = 0.0015;

/**
 * Triangulation algorithm. Does actual direction calculation.
 * 'ts_a' is the timestamp of a ping arriving at receiver A
 * 'ts_b' is the timestamp of a ping arriving at receiver B
 * ...
 * 'result' is an out param
 *
 *
 *            Hydrophone Configuration
 *                         y
 *                         |
 *                         |
 *                         |
 *                  b*-----+-----*a
 *                   |     |     |
 *           --------------+--------------x
 *                   |     |     |
 *                  c*-----+-----*d
 *
 *                   |---  L  ---|
 *
 *
 */


/*
* Wrapper function for Triangulation calculation.
* First argument(s) point to "ping" or similar structs
* 'result' is an out parameter and is a matrix (or other)
*/

/* Update as appropriate
 * "ping" arguments here are the time stamps for microphones
 * a, b, c and d, where that letter corresponds to the diagram
 * at line 44
 * for the sake of testing, they are represented here as
 * variables, NOT as arguments.  this will be updated as more
 * work is done and what that argument is is figured out.
*/

int triangulation(struct ping a, struct ping b, struct ping c, struct ping d, crab_event *result){
  int ret = triangulation_helper(a.start, b.start, c.start, d.start, result);

  printResult(result);

  return ret;

}
/*
 * time stamp 1 - 4 are the timestamps in order of appearance.
 * result is the structure
 */
int triangulation_helper(unsigned long ts_a, unsigned long ts_b, unsigned long ts_c, unsigned long ts_d, crab_event *result){
  /* x, y, and z directions */
  double x;
  double y;
  double z = 1;
  double N;
  /*changes in time bewtween various hydrophones*/
  double delta_1 = (double)ts_b - (double)ts_a;
  double delta_2 = (double)ts_c - (double)ts_a;
  double delta_3 = (double)ts_d - (double)ts_a;


  //Set biggest delta
  double max_delta = (double) (1 / S_USER) * sqrt(2 * pow(2 * R_USER, 2.0));
  //find biggest delta
  double longest_delta = fmax(fmax(abs(delta_1), abs(delta_2)), abs(delta_3));
  //check deltas are in reasonable range for good data
  if (abs(longest_delta) > max_delta){
    return -1;
  }

  if (delta_1 == 0 || delta_3 == 0){
    //Directly above crab
    if(delta_1 == 0 && delta_2 == 0 && delta_3 == 0){
      x = 0;
      y = 0;
      //z unknown. Can use last known Z or something like 1
      z = 1;
    }
    //Crab on x-axis
    if (delta_3 == 0){
      return -1;
    }
    //Crab on y-axis
    if (delta_1 == 0){
      return -1;
    }
  }
  else{
    N = calcN(delta_1, delta_2, delta_3);
    x = calcX(delta_1, N);
    y = calcY(delta_3, N);
    z = calcZ(N, x, y);
    double r = calcR(x, y);
    double theta = calcAngle(x,y);
    result->N = N;
    result->r_m = r;
    result->theta = theta;
    result->z_m = z;
  }
  return 1;
}

/*
 *     s(delta_2^2 - delta_1^2 - delta_3^2)
 * N = -------------------------------------
 *     2(delta_1 + delta_3 - delta_2)
 */
double calcN(double delta_1, double delta_2, double delta_3){
  long double N;
  if ((delta_1 + delta_3 - delta_2) == 0){
    //fprintf(stderr, "Small delta \n");
    N = (S_USER*(pow(delta_2,2.0) - pow(delta_1,2.0) - pow(delta_3,2.0)))  /  (2.0);
  } else if ((pow(delta_2,2.0) - pow(delta_1,2.0) - pow(delta_3,2.0)) == 0) {
    //fprintf(stderr, "Small bottom \n");
    //not sure needed
    N = (S_USER*1.0)  /  (2.0*(delta_1 + delta_3 - delta_2));
  } else {
    N = (S_USER*(pow(delta_2,2.0) - pow(delta_1,2.0) - pow(delta_3,2.0)))  /  (2.0*(delta_1 + delta_3 - delta_2));
  }
  return N;
}

/*
 *     (s * delta_1)^2 + 2 * s * delta_1 *N
 * x = -------------------------------------
 *                 4 * r
 */
double calcX(double delta_1, double N){
  double x = (double)(pow((S_USER * delta_1),2.0) + (2.0*S_USER*delta_1*N))/(4.0*R_USER);
  return x;
}

/*
 *     (s * delta_3)^2 + 2 * s * delta_3 *N
 * y = -------------------------------------
 *                 4 * r
 */
double calcY(double delta_3, double N){
  double y = (double)(pow((S_USER * delta_3),2.0) + (2.0*S_USER*delta_3*N))/(4.0*R_USER);
  return y;
}

/*
 *
 * z = sqrt( N^2 -  (x - r)^2  -  (y - r)^2)
 *
 */
double calcZ(double N, double x, double y){
  double z = (double)sqrt(abs(pow(N,2.0) - pow(x - R_USER , 2.0) - pow(y - R_USER, 2.0)));
  return z;
}

/*
 *
 * r = sqrt(x^2 + y^2)
 *
 */
double calcR(double x, double y){
  double r = (double)sqrt(pow(x,2.0) + pow(y,2.0));
  return r;
}

/*
 *
 * theta = tan^(-1)(y/x)
 * radians to degrees: degrees = radians * (180/pi)
 * theta_degrees = tan^(-1)(y/x) * 180/pi
 */
double calcAngle(double x, double y){
  double pi = (double)atan(1)*4;
  double theta_degrees = (double)atan2(y,x) * 180 / pi;
  return theta_degrees;
}

/*debugging function prints data structs */
void printResult(crab_event *result){
  fprintf(stderr, "r = %lf\n", result->r_m);
  fprintf(stderr, "theta = %lf\n", result->theta);
  fprintf(stderr, "z = %lf\n", result->z_m);
}

double calcSpeedOfSound(double temp, double salinity, double depth) {
    return 1449.2 + (4.6*temp)
                  - (5.5 * pow(10.0,-2.0)) * pow(temp, 2.0)
                  + (1.34 - (pow(10,-2.0)*temp)) * (salinity-35)
                  + 1.6 * pow(10.0,-2.0) * depth;
}
