/* file: lis-4.c
* author: David De Potter
* description: longest increasing subsequence
* solution in linearithmic time
*/ 

#include <stdlib.h>
#include <stdio.h>

void *safeCalloc (int n, int size) {
  /* allocates n elements of size size, initializing them to 0, and
     checks whether the allocation was successful */
  void *ptr = calloc(n, size);
  if (ptr == NULL) {
    printf("Error: calloc(%d, %d) failed. Out of memory?\n", n, size);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

int *newArray (int n) {
  /* creates an array of size n */
  return safeCalloc(n, sizeof(int));
}

int computeLIS (int *arr, int n, int *dp, int *prev) {
  /* computes the LIS and returns the length of the LIS */
  dp[0] = 0;
  int len = 1;
  for (int i = 1; i < n; i++) {
    if (arr[i] > arr[dp[len-1]]) {
      prev[i] = dp[len-1]; 
      dp[len++] = i;
    } else {
      int low = 0, high = len-1;
      while (low < high) {
        int mid = low + (high - low) / 2;
        if (arr[dp[mid]] < arr[i]) low = mid + 1;
        else high = mid;
      }
      dp[low] = i;
      if (low > 0) prev[i] = dp[low-1];
    }
  }
  return len;
}

void reconstructLIS (int *arr, int *prev, int p, int last, int *lis) {
  /* reconstructs the LIS by following the parent pointers */
  for (int i = last; i >= 0; i--) {
    lis[i] = arr[p]; 
    p = prev[p];
  }
}

void printArray (int *arr, int n) {
  // prints an array of size n
  printf("[");
  for (int i = 0; i < n; i++) {
    printf("%d", (arr)[i]);
    if (i < n-1) printf(", ");
  }
  printf("]\n");
}

int main (int argc, char *argv[]) {
  int arr[] = {12, 8, 2, 1, 4, 2, 9, 10, 18, 15, 12, 7, 3, 1, 
               20, 14, 30, 12, 24, 40, 5, 10, 5, 26, 11, 2};
  int n = 26;
  int *dp = newArray(n);
  int *prev = newArray(n);
  int lisLen = computeLIS(arr, n, dp, prev);
  int *lis = newArray(lisLen);
  reconstructLIS(arr, prev, dp[lisLen-1], lisLen-1, lis);
  printf("The given array is:\n");
  printArray(arr, n);
  printf("The length of the longest increasing subsequence is %d.\n", lisLen);
  printf("A possible LIS is:\n");
  printArray(lis, lisLen);
  free(lis); free(prev); free(dp);
  return 0;
}