#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <time.h>

#include "direction.h"
#define R 1 /*  "r"  the length of the hydrophone square          */
#define R_USER 1
#define _ENABLE_RANDOM_TIME_NOISE 1 /* 1 to add +/-6us to timestamps, else 0 */

const double S_USER = 1.5/1000;
using namespace std;


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

typedef struct{
  unsigned long ts_a;
  unsigned long ts_b;
  unsigned long ts_c;
  unsigned long ts_d;
} actualTime;

/* PROTOTYPES */
int calcDist(double x, double y, double z, data *result, timestamps *times);
void printTimes(timestamps *times);

void convertTimestamps(timestamps *times, actualTime *actual);
void difference( data *actual, data *result, data *diffResult);
int triangulation_helper(unsigned long ts_a, unsigned long ts_b, unsigned long ts_c, unsigned long ts_d, data *result);
/* END PROTOTYPES */

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

int main(int argc, const char* argv[]) {
  data actual;
  timestamps actualTimes;
  actualTime intTimes;
  data result;
  data diffResult;
  double x1;
  double x2;
  double x3;
  double y1;
  double y2;
  double y3;
  double z1;
  double z2;
  double z3;
  double N1;
  double N2;
  double N3;
  double r1;
  double r2;
  double r3;
  double t1;
  double t2;
  double t3;
  int goodcount = 0;
  int badcount = 0;
  // Error is between 1-5m off for abs(N) etc
  int error1 = 0;
  int error5 = 0;
  int error10 = 0;
  int error20up = 0;
  std::ofstream output;
  output.open ("data.csv");
  output << "x,y,z,N,r,theta,calcX,calcY, calcZ,calcN,calcR,calcTheta, difX,difZ,difY,difN,difR,difTheta\n";
  for (double i = -10.0; i <= 10.0; i += 0.1) {
    for (double j = -10.0; j <= 10.0; j += 0.1) {
      for (double k = 3.0; k <= 10.0; k += 0.1) {
        x1 = i;
        y1 = j;
        z1 = k;
        calcDist(x1, y1, z1, &actual, &actualTimes);
        N1 = actual.N;
        r1 = actual.r;
        t1 = actual.theta;
        convertTimestamps( &actualTimes, &intTimes);
        unsigned long ts_a = intTimes.ts_a;
        unsigned long ts_b = intTimes.ts_b;
        unsigned long ts_c = intTimes.ts_c;
        unsigned long ts_d = intTimes.ts_d;
        if(_ENABLE_RANDOM_TIME_NOISE){
            ts_a += (rand() % 6 + -6);
            ts_b += (rand() % 6 + -6);
            ts_c += (rand() % 6 + -6);
            ts_d += (rand() % 6 + -6);
        }
        triangulation_helper(ts_a, ts_b, ts_c, ts_d, &result);
        x2 = result.x;
        y2 = result.y;
        z2 = result.z;
        N2 = result.N;
        r2 = result.r;
        t2 = result.theta;
        difference(&actual, &result, &diffResult);
        x3 = diffResult.x;
        y3 = diffResult.y;
        z3 = diffResult.z;
        N3 = diffResult.N;
        r3 = diffResult.r;
        t3 = diffResult.theta;
        if ((abs(N3) > 1) && (abs(N3) < 5)) {
          error1++;
          badcount++;
          output << x1 << "," << y1 << "," << z1 << "," << N1 << "," << r1 << "," << t1 << "," <<
          x2 << "," << y2 << "," << z2 << "," << N2 << "," << r2 << "," << t2 << ","
          << x3 << "," << y3 << "," << z3 << "," << N3 << "," << r3 << "," << t3 << std::endl;
        } else if ((abs(N3) > 5) && (abs(N3) < 10)) {
          error5++;
          badcount++;
          output << x1 << "," << y1 << "," << z1 << "," << N1 << "," << r1 << "," << t1 << "," <<
          x2 << "," << y2 << "," << z2 << "," << N2 << "," << r2 << "," << t2 << ","
          << x3 << "," << y3 << "," << z3 << "," << N3 << "," << r3 << "," << t3 << std::endl;
        } else if ((abs(N3) > 10) && (abs(N3) < 20)) {
          error10++;
          badcount++;
          output << x1 << "," << y1 << "," << z1 << "," << N1 << "," << r1 << "," << t1 << "," <<
          x2 << "," << y2 << "," << z2 << "," << N2 << "," << r2 << "," << t2 << ","
          << x3 << "," << y3 << "," << z3 << "," << N3 << "," << r3 << "," << t3 << std::endl;
        } else if (abs(N3) > 20) {
          error20up++;
          badcount++;
          output << x1 << "," << y1 << "," << z1 << "," << N1 << "," << r1 << "," << t1 << "," <<
          x2 << "," << y2 << "," << z2 << "," << N2 << "," << r2 << "," << t2 << ","
          << x3 << "," << y3 << "," << z3 << "," << N3 << "," << r3 << "," << t3 << std::endl;
        } else {goodcount++;}
      }
    }
  }
  double percent = (double)badcount/(goodcount + badcount);
  fprintf(stderr, "Percent bad %lf\n", percent);
  fprintf(stderr, "Good %d\n", goodcount);
  fprintf(stderr, "Bad %d\n", badcount);
  fprintf(stderr, "Error between 1 and 5 %d\n", error1);
  fprintf(stderr, "Error between 5 and 10 %d\n", error5);
  fprintf(stderr, "Error between 10 and 20 %d\n", error10);
  fprintf(stderr, "Error greater than 20 %d\n", error20up);
  fprintf(stderr, "Error percent between 1 and 5 %f\n", (double)error1/(goodcount + badcount));
  fprintf(stderr, "Error percent between 5 and 10 %f\n", (double)error5/(goodcount + badcount));
  fprintf(stderr, "Error percent between 10 and 20 %f\n", (double)error10/(goodcount + badcount));
  fprintf(stderr, "Error percentgreater than 20 %f\n", (double)error20up/(goodcount + badcount));
  output.close();
  return 0;
}

void convertTimestamps(timestamps *times, actualTime *actual) {
  actual->ts_a = (unsigned long)(times->ts_a);
  actual->ts_b = (unsigned long)(times->ts_b);
  actual->ts_c = (unsigned long)(times->ts_c);
  actual->ts_d = (unsigned long)(times->ts_d);
}

void difference( data *actual, data *result, data *diff) {
  diff->x = (actual->x - result->x);
  diff->y = (actual->y - result->y);
  diff->z = abs(actual->z - result->z);
  diff->N = abs(actual->N - result->N);
  diff->r = (actual->r - result->r);
  diff->theta = (actual->theta - result->theta);
}

int triangulation_helper(unsigned long ts_a, unsigned long ts_b, unsigned long ts_c, unsigned long ts_d, data *result){
  /* x, y, and z directions */
  double x;
  double y;
  double z = 1;
  double N;
  /*changes in time bewtween various hydrophones*/
  double delta_1 = (double)ts_b - (double)ts_a;
  double delta_2 = (double)ts_c - (double)ts_a;
  double delta_3 = (double)ts_d - (double)ts_a;

  //fprintf(stderr, "delta_1 %lf\n", delta_1);
  //fprintf(stderr, "delta_2 %lf\n", delta_2);
  //fprintf(stderr, "delta_3 %lf\n", delta_3);


  //Set biggest delta
  double max_delta = (double) (1 / S_USER) * sqrt(2 * pow(2 * R_USER, 2.0));
  //fprintf(stderr, "max_delta %f\n", max_delta);
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
    result->r = r;
    result->theta = theta;
    result->N = N;
    result->x = x;
    result->y = y;
    result->z = z;
  }
  return 1;
}

/*debugging function prints data structs */
void printResult(data *result){
  fprintf(stderr, "x = %lf\n", result->x);
  fprintf(stderr, "y = %lf\n", result->y);
  fprintf(stderr, "z = %lf\n", result->z);
  fprintf(stderr, "N = %lf\n", result->N);
  fprintf(stderr, "r = %lf\n", result->r);
  fprintf(stderr, "theta = %lf\n", result->theta);
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
  //printTimes(times);

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
