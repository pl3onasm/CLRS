/* file: msp-2.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: Maximum subarray problem using 
    a divide and conquer approach
   time complexity: O(nlogn)
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
  int low, high, sum;
} Sub;

void printResult (int arr[], Sub max) {
  /* prints the result */
  printf("Maximum subarray: \n["); 
  for (int i = max.low; i <= max.high; ++i) {
    printf("%d", arr[i]);
    if (i < max.high) printf(", ");
  }
  printf("]\n");
  printf("Maximum sum: %d\n", max.sum);
}

void getMaxCrossSub (int arr[], int mid, Sub *max) {
  /* determines the maximum subarray crossing the midpoint */
  int sum = 0;
  int leftSum = INT_MIN, rightSum = INT_MIN;
  int maxLeft = 0, maxRight = 0;
  for (int i = mid; i >= max->low; --i) {
    sum += arr[i];
    if (sum > leftSum) {
      leftSum = sum;
      maxLeft = i;
    }
  }
  sum = 0;
  for (int j = mid + 1; j <= max->high; ++j) {
    sum += arr[j];
    if (sum > rightSum) {
      rightSum = sum;
      maxRight = j;
    }
  }
  max->low = maxLeft;
  max->high = maxRight;
  max->sum = leftSum + rightSum;
}

void getMaxSub (int arr[], Sub *max) {
  /* computes the maximum subarray */
  int low = max->low, high = max->high;
  if (low == high) max->sum = arr[low];
  else {
    int mid = (low + high) / 2;
    Sub left = {low, mid, 0};
    getMaxSub(arr, &left);
    Sub right = {mid + 1, high, 0};
    getMaxSub(arr, &right);
    Sub cross = {low, high, 0};
    getMaxCrossSub(arr, mid, &cross);

    if (left.sum >= right.sum && left.sum >= cross.sum) *max = left;
    else if (right.sum >= left.sum && right.sum >= cross.sum) *max = right;
    else *max = cross;
  }
}

int main(int argc, char *argv[]) {
  int example[] = {1, -3, 2, 14, -9, 16, 7, -20, 3, -14, 5, 
                  -1, 3, -5, -4, 17, -8, 9, -10, 11, -12, 13,
                  -14, 15, -20, 17, -18, 19, -20, 16, -2, 14};

  int n = sizeof(example) / sizeof(int);
  Sub max = {0, n-1, 0};
  
  getMaxSub(example, &max);
  printResult(example, max);

  return 0;
}