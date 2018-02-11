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
#define S_USER 1460/*  "s" f(salinity, temperature), to be defined later */
#define R_USER 1 /*  "r"  the length of the hydrophone square          */


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
double calcN(unsigned long delta_1, unsigned long delta_2, unsigned long delta_3);
double calcX(unsigned long delta_2, double N);
double calcY(unsigned long delta_1, double N);
double calcZ(double N, double x, double y);
void printResult(xyz *result);
void selectionSort(unsigned long arr[], int n);
void swap(unsigned long*xp, unsigned long *yp);

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
  unsigned long ts_a = 24;
  unsigned long ts_b = 85;
  unsigned long ts_c = 6666666;
  unsigned long ts_d = 3985;

  /* sort the time stamps */
  unsigned long arr[4] = {ts_a, ts_b, ts_c, ts_d};
  selectionSort(arr, 4);
 /* declaration of variables to be passed to triangulation_helper */
  unsigned long ts_1 = arr[0];
  unsigned long ts_2 = arr[1];
  unsigned long ts_3 = arr[2];
  unsigned long ts_4 = arr[3];

  //fprintf(stderr, "%ld, %ld, %ld, %ld \n", ts_1, ts_2, ts_3, ts_4);

  //xyz result;

  triangulation_helper(ts_1, ts_2, ts_3, ts_4, result);

  printResult(result);

  return 1;

}
/*
 * time stamp 1 - 4 are the timestamps in order of appearance.
 * result is the structure
 */
int triangulation_helper(unsigned long ts_1, unsigned long ts_2, unsigned long ts_3, unsigned long ts_4, xyz *result){
  /* x, y, and z directions */
  fprintf(stderr, "IN TRIG_H\n");
  double x;
  double y;
  double z = 1; //
  double N;
  /*changes in time bewtween various hydrophones*/
  unsigned long delta_1 = ts_1 - ts_4;
  unsigned long delta_2 = ts_2 - ts_4;  //these are out of order in alg doccumentation,
  unsigned long delta_3 = ts_3 - ts_4;  //double check with chloe that this is what she meant

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
double calcN(unsigned long delta_1, unsigned long delta_2, unsigned long delta_3){
  double N = (double)(S_USER*(pow(delta_1,2.0) + pow(delta_2,2.0) - pow(delta_3,2.0)))  /  (2.0*(delta_3 - delta_1 - delta_2));
  return N;
}

/*
 *     (s * delta_2)^2 + 2 * s * delta_2 *N
 * x = -------------------------------------
 *                 2 * r
 */
double calcX(unsigned long delta_2, double N){
  double x = (double)(pow((S_USER * delta_2),2.0) + (2.0*S_USER*delta_2*N))/(2.0*R_USER);
  return x;
}

/*
 *     (s * delta_1)^2 + 2 * s * delta_1 *N
 * y = -------------------------------------
 *                 -2 * r
 */
double calcY(unsigned long delta_1, double N){
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

/* sorting stuff */

void swap(unsigned long *xp, unsigned long *yp){
    unsigned long temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void selectionSort(unsigned long arr[], int n){
    int i, j, min_idx;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < n-1; i++)
    {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i+1; j < n; j++)
          if (arr[j] < arr[min_idx])
            min_idx = j;

        // Swap the found minimum element with the first element
        swap(&arr[min_idx], &arr[i]);
    }
}
