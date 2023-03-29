/* file: cut-rod2.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: top-down approach with memoization
   The time complexity is in O(n²).
*/ 

#include <stdio.h>
#include <stdlib.h>

#define MAX(a,b) ((a)>(b)?(a):(b));

int cut_rod (int prices[], int n, int revenues[]){
  if (revenues[n] >= 0)  // return the stored value 
    return revenues[n];
  int q = -1;
  if (n == 0) q = 0;
  else {    // compute the maximum revenue
    for (int i = 1; i <= n; i++) {
      q = MAX(q, prices[i] + cut_rod(prices, n - i, revenues));
    }
  }
  revenues[n] = q;  // store the computed value
  return q;
}

int main (int argc, char *argv[]) {
  int prices[] = {0, 1, 5, 8, 9, 10, 17, 17, 
                  20, 24, 30, 32, 35, 39, 43, 
                  43, 45, 49, 50, 54, 57, 60, 
                  65, 68, 70, 74, 80, 81, 84, 
                  85, 87, 91, 95, 99, 101, 104, 
                  107, 112, 115, 116, 119}; 
  int revenues[40];
  for (int i = 0; i < 40; i++) 
    revenues[i] = -1; // initialize all revenues to -1

  printf("The maximum revenue is %d for a rod of length"
        " %d, and %d for a rod of length %d.\n", 
        cut_rod(prices, 22, revenues), 22, 
        cut_rod(prices, 35, revenues), 35);

  return 0;
}
