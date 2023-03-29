/* file: cut-rod1.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description:
    This is a naive recursive implementation of 
    rod cutting. By induction we can prove T(n) = 2ⁿ, 
    where n is the length of the rod.
    The time complexity is therefore in O(2ⁿ).
*/ 

#include <stdio.h>
#include <stdlib.h>

#define MAX(a,b) ((a)>(b)?(a):(b));

int cut_rod(int prices[], int n) {
  if (n == 0)
    return 0;
  int q = -1;
  for (int i = 1; i <= n; i++) 
    q = MAX(q, prices[i] + cut_rod(prices, n - i));
  return q;
}

int main (int argc, char *argv[]) {
  int prices[] = {0, 1, 5, 8, 9, 10, 17, 17, 
                  20, 24, 30, 32, 35, 39, 43, 
                  43, 45, 49, 50, 54, 57}; 

  printf("The maximum revenue is %d for a rod of length"
        " %d, and %d for a rod of length %d.\n", 
        cut_rod(prices, 10), 10, cut_rod(prices, 15), 15);

  return 0;
}
