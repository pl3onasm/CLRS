/* file: msp-1.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: Maximum subarray problem, brute force 
   time complexity: Θ(n²)
*/

#include <stdio.h>
#include <stdlib.h>

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

int getMaxSub(int arr[], int n, int *l, int *h) {
  /* determines the maximum subarray */
  int max = 0;  // max sum
  for (int i = 0; i < n; ++i) {
    int sum = 0;
    for (int j = i; j < n; ++j) {
      sum += arr[j];
      if (sum > max) {
        max = sum;
        *l = i;
        *h = j;
      }
    }
  }
  return max;
}

int main(int argc, char *argv[]) {
  int example[] = {1, -3, 2, 14, -9, 16, 7, -20, 3, -14, 5, -1, 3, -5, -4,
    17, -8, 9, -10, 11, -12, 13, -14, 15, -20, 17, -18, 19, -20, 16, -2, 14};

  int n = sizeof(example) / sizeof(int);
  int l = 0, h = n - 1;  // low and high indices
  
  int max = getMaxSub(example, n, &l, &h);

  printResult(example, l, h, max);

  return 0; 
}
  

  