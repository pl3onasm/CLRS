/* file: cut-rod3.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: bottom-up approach to rod cutting
   Time complexity: O(n²).
*/ 

#include <stdio.h>
#include <stdlib.h>

#define MAX(a,b) ((a)>(b)?(a):(b));

int cut_rod (int prices[], int n, int revenues[]) {
  if (revenues[n] >= 0)  // return the stored value 
    return revenues[n];
  for (int j = 1; j <= n; j++) {
    int q = -1;
    for (int i = 1; i <= j; i++) {
      q = MAX(q, prices[i] + revenues[j - i]);  // no recursive call
    }
    revenues[j] = q;  // store the computed value
  }
  return revenues[n];
}

int main (int argc, char *argv[]) {
  int prices[] = {0, 1, 5, 8, 9, 10, 17, 17, 
                  20, 24, 30, 32, 35, 39, 43, 
                  43, 45, 49, 50, 54, 57, 60, 
                  65, 68, 70, 74, 80, 81, 84, 
                  85, 87, 91, 95, 99, 101, 104, 
                  107, 112, 115, 116, 119}; 
  int revenues[40];
  revenues [0] = 0;   // revenue for a rod of length 0 is 0
  for (int i = 1; i < 40; i++) {
    revenues[i] = -1; // initialize remaining values to -1
  }
  
  printf("The maximum revenue is %d for a rod of length"
        " %d, and %d for a rod of length %d.\n", 
        cut_rod(prices, 25, revenues), 25, 
        cut_rod(prices, 39, revenues), 39);

  return 0;
}
