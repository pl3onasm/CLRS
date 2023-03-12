/* file: msp-3.c
   author: David De Potter
   description: Maximum subarray problem using 
    an iterative linear approach
   time complexity: O(n)
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void printResult (int arr[], int l, int h, int max) {
  /* prints the result */
  printf("Maximum subarray: \n[");
  for (int i = l; i <= h; ++i) {
    printf("%d", arr[i]);
    if (i < h) printf(", ");
  }
  printf("]\n");
  printf("Maximum sum: %d\n", max);
}

int maxSubarray(int arr[], int n, int *l, int *h) {
  /* computes the maximum subarray */
  int max = INT_MIN, sum = 0, low = 0;
  for (int i = 0; i < n; ++i) {
    sum += arr[i];
    if (sum > max) { // update max
      max = sum;
      *h = i;
      *l = low;
    }
    if (sum < 0) {   // reset sum
      sum = 0;
      low = i+1;
    }
  }
  return max;
}

int main(int argc, char *argv[]) {
  int example[] = {1, -3, 2, 14, -9, 16, 7, -20, 3, -14, 5, -1, 3, -5, -4,
    17, -8, 9, -10, 11, -12, 13, -14, 15, -20, 17, -18, 19, -20, 16, -2, 14};

  int n = sizeof(example) / sizeof(int);
  int l = 0, h = n-1;  // low and high indices
  
  int max = maxSubarray(example, n, &l, &h);
  printResult(example, l, h, max);

  return 0; 
}