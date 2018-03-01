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
//#define S_USER 1500/*  "s" f(salinity, temperature), to be defined later */
#define R_USER 1 /*  "r"  the length of the hydrophone square          */
const double S_USER = 0.0015;

typedef struct {
  double x;
  double y;
  double z;
} xyz;


 /*
 * Wrapper function for Triangulation calculation.
 * First argument(s) point to "ping" or similar structs
 * 'result' is an out parameter and is a matrix (or other)
 */
int triangulation( /* 'ping' arguments TBD, */ xyz *result);


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
 *                  b*-----+-----*c
 *                   |     |     |
 *           ---------r----+--------------x
 *                   |     |     |
 *                  a*-----+-----*d
 *                         |
 *                         |
 *                         |
 *
 */
int triangulation_helper(unsigned long ts_a, unsigned long ts_b, unsigned long ts_c, unsigned long ts_d, xyz *result);

/*
 * Helper functions calculate various calculations
*/
double calcN(long delta_1, long delta_2, long delta_3);
double calcX(long delta_2, double N);
double calcY(long delta_1, double N);
double calcZ(double N, double x, double y);
void printResult(xyz *result);

int main(int argc, const char* argv[]) {
  xyz result;
  triangulation(&result);

  return 1;
}

/* Update as appropriate
 * "ping" arguments here are the time stamps for microphones
 * a, b, c and d, where that letter corresponds to the diagram
 * at line 44
 * for the sake of testing, they are represented here as
 * variables, NOT as arguments.  this will be updated as more
 * work is done and what that argument is is figured out.
*/

int triangulation( /* 'ping' arguments TBD, */ xyz *result){
  fprintf(stderr, "IN TRIG\n");
  /*  These will be removed when 'ping arguments' is a real thing*/
  unsigned long ts_a = 52283;
  unsigned long ts_b = 52537;
  unsigned long ts_c = 52376;
  unsigned long ts_d = 52121;

  triangulation_helper(ts_a, ts_b, ts_c, ts_d, result);

  printResult(result);

  return 1;

}
/*
 * time stamp 1 - 4 are the timestamps in order of appearance.
 * result is the structure
 */
int triangulation_helper(unsigned long ts_a, unsigned long ts_b, unsigned long ts_c, unsigned long ts_d, xyz *result){
  /* x, y, and z directions */
  fprintf(stderr, "IN TRIG_H\n");
  double x;
  double y;
  double z = 1; //
  double N;
  /*changes in time bewtween various hydrophones*/
  long delta_1 = ts_a - ts_d;
  long delta_2 = ts_c - ts_d;
  long delta_3 = ts_b - ts_d;
  fprintf(stderr, "delta_1 %ld\n", delta_1);
  fprintf(stderr, "delta_2 %ld\n", delta_2);
  fprintf(stderr, "delta_3 %ld\n", delta_3);

  if(delta_1 == 0 || delta_2 == 0){
    fprintf(stderr, "IN IF\n");
    //"z is the last known z.  may need user input for first one.
    //Alternatively, throw out if first data point hits this case
    //and wait for the next signal"
    if(delta_1 == 0 && delta_2 == 0){
      x = 0;
      y = 0;
      z = z;
    }
    if(delta_1 == 0 && delta_2 > 0){
      /*
      x = something
      */
      y = 0;
    }
    if(delta_1 == 0 && delta_2 < 0){
      /*
      x = something
      */
      y = 0;
    }
    if(delta_2 == 0 && delta_1 > 0){
      z = 0;
      /*
      y = something
      */
    }
    if(delta_2 == 0 && delta_1 < 0){
      z = 0;
      /*
      y = something
      */
    }
  }
  else{
    fprintf(stderr, "IN ELSE\n");
    N = calcN(delta_1, delta_2, delta_3);
    fprintf(stderr, "%lf\n", N);
    x = calcX(delta_2, N);
    fprintf(stderr, "%lf\n", x);
    y = calcY(delta_1, N);
    fprintf(stderr, "%lf\n", y);
    z = calcZ(N, x, y);
    fprintf(stderr, "%lf\n", z);

    result->x = x;
    result->y = y;
    result->z = z;

  }


  return 1;
}

/*
 *     s(delta_1^2 + delta_2^2 - delta_3^2)
 * N = -------------------------------------
 *     2(delta_3 - delta_1 - delta_2)
 */
double calcN(long delta_1, long delta_2, long delta_3){
  fprintf(stderr, "delta_1 %ld\n", delta_1);
  fprintf(stderr, "delta_2 %ld\n", delta_2);
  fprintf(stderr, "delta_3 %ld\n", delta_3);
  long double topN = (S_USER*(pow(delta_1,2.0) + pow(delta_2,2.0) - pow(delta_3,2.0)));
  fprintf(stderr, "TopN %Lf\n", topN);
  long double botN = (2.0*(delta_3 - delta_1 - delta_2));
  fprintf(stderr, "BotN %Lf\n", botN);
  long double testN = topN / botN;
  fprintf(stderr, "testN %Lf\n", testN);
  long double N = (S_USER*(pow(delta_1,2.0) + pow(delta_2,2.0) - pow(delta_3,2.0)))  /  (2*(delta_3 - delta_1 - delta_2));
  fprintf(stderr, "N %Lf\n", N);
  return N;
}

/*
 *     (s * delta_2)^2 + 2 * s * delta_2 *N
 * x = -------------------------------------
 *                 2 * r
 */
double calcX(long delta_2, double N){
  double x = (double)(pow((S_USER * delta_2),2.0) + (2.0*S_USER*delta_2*N))/(2.0*R_USER);
  return x;
}

/*
 *     (s * delta_1)^2 + 2 * s * delta_1 *N
 * y = -------------------------------------
 *                 -2 * r
 */
double calcY(long delta_1, double N){
  //double y = (double)(pow((S_USER * delta_1),2.0) + (2.0*S_USER*delta_1*N))/(-2.0*R_USER);
  double y = (double)(pow((S_USER * delta_1),2.0) + (2.0*S_USER*delta_1*N))/(-2.0*R_USER);
  return y;
}

/*
 *
 * z = sqrt( N^2 -  (x - r/2)^2  -  (y + r/2)^2)
 *
 */
double calcZ(double N, double x, double y){
  double z = (double)sqrt(pow(N,2.0) - pow(x - R_USER/2.0 , 2.0) - pow(y + R_USER/2.0 , 2.0));
  return z;
}

/*debugging function prints xyz structs */
void printResult(xyz *result){
  fprintf(stderr, "x = %lf\n", result->x);
  fprintf(stderr, "y = %lf\n", result->y);
  fprintf(stderr, "z = %lf\n", result->z);
}
