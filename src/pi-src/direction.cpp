/**
 *  Triangulation Algorithm Implementation
 *  Elizabeth Schoen February 2018
 *  February 2018
 *
 */
#include <stdlib.h>
#include <math.h>
#define S_USER 1460/*  "s" f(salinity, temperature), to be defined later */
#define R_USER 1 /*  "r"  the length of the hydrophone square          */


typedef struct {
  double x;
  double y;
  double z;
} xyz;


 /*
 *
 *
 *
 *
 *
 *
 *
 *
 * Wrapper function for Triangulation calculation.
 * First argument(s) point to "ping" or similar structs
 * 'result' is an out parameter and is a matrix (or other)
 */
int dirvec( /* 'ping' arguments TBD, */ xyz *result);


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
int dirvec_helper(long ts_a, long ts_b, long ts_c, long ts_d, xyz *result);

/*
 * Helper functions calculate various calculations
*/
double calcN(long delta_1, long delta_2, long delta_3);
double calcX(double delta_2, double N);
double calcY(double delta_1, double N);
double calcZ(double N, double x, double y);

int main(int argc, const char* argv[]) {
  return 1;
}

int dirvec_helper(long ts_1, long ts_2, long ts_3, long ts_4, xyz *result){
  /* x, y, and z directions */
  double x;
  double y;
  double z;
  double N;
  /*changes in time bewtween various hydrophones*/
  long delta_1 = ts_1 - ts_4;
  long delta_2 = ts_2 - ts_4;  //these are out of order in alg doccumentation,
  long delta_3 = ts_3 - ts_4;  //double check with chloe that this is what she meant

  if(delta_1 == 0 || delta_2 == 0){
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
    else{
      N = calcN(delta_1, delta_2, delta_3);
      x = calcX(delta_2, N);
      y = calcY(delta_1, N);
      z = calcZ(N, x, y);

      result->x = x;
      result->y = y;
      result->z = z;

    }


  }



  return 1;
}

/*
 *     s(delta_1^2 + delta_2^2 - delta_3^2)
 * N = -------------------------------------
 *     2(delta_3 - delta_1 - delta_2)
 */
double calcN(long delta_1, long delta_2, long delta_3){
  double N = (S_USER*(pow(delta_1,2.0) + pow(delta_2,2.0) - pow(delta_3,2.0)))  /  (2.0*(delta_3 - delta_1 - delta_2));
  return N;
}

/*
 *     (s * delta_2)^2 + 2 * s * delta_2 *N
 * x = -------------------------------------
 *                 2 * r
 */
double calcX(double delta_2, double N){
  double x = (pow((S_USER * delta_2),2.0) + (2.0*S_USER*delta_2*N))/(2.0*R_USER);
  return x;
}

/*
 *     (s * delta_1)^2 + 2 * s * delta_1 *N
 * y = -------------------------------------
 *                 -2 * r
 */
double calcY(double delta_1, double N){
  double y = (pow((S_USER * delta_1),2.0) + (2.0*S_USER*delta_1*N))/(-2.0*R_USER);
  return y;
}

/*
 *
 * z = sqrt( N^2 -  (x - r/2)^2  -  (y + r/2)^2)
 *
 */
double calcZ(double N, double x, double y){
  double z = sqrt(pow(N,2.0) - pow(x - R_USER/2.0 , 2.0) - pow(y + R_USER/2.0 , 2.0));
  return z;
}
