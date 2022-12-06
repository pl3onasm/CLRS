/* file: lis-1.c
* author: David De Potter
* description: longest increasing subsequence
* naive recursive implementation
*/ 

#include <stdlib.h>
#include <stdio.h>

void *safeMalloc (int n) {
  /* allocates n bytes of memory and checks whether the allocation
     was successful */
  void *ptr = malloc(n);
  if (ptr == NULL) {
    printf("Error: malloc(%d) failed. Out of memory?\n", n);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void getMaxLis (int *a, int *lis, int n, int idx, int subLen, int *maxLis, int *maxLen) {
  /* computes the LIS of a and stores it in lis */
  if (idx == n) {
    if (subLen > *maxLen) {
      *maxLen = subLen;
      for (int i = 0; i < subLen; i++)  // copy lis to maxLis
        maxLis[i] = lis[i];
    }
    return;
  }
  if (subLen == 0 || a[idx] > lis[subLen-1]) {
    lis[subLen] = a[idx];
    getMaxLis(a, lis, n, idx+1, subLen+1, maxLis, maxLen);
  }
  getMaxLis(a, lis, n, idx+1, subLen, maxLis, maxLen);
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
  int arr[] = {12, 8, 1, 4, 2, 9, 10, 18, 15, 7, 
             3, 20, 14, 30, 24, 40, 5, 10, 26, 11};
  int n = 20; 
  int *lis = safeMalloc(n * sizeof(int));
  int *maxLis = safeMalloc(n * sizeof(int));
  int maxLen = 0;
  getMaxLis(arr, lis, n, 0, 0, maxLis, &maxLen);
  printf("The length of the longest increasing subsequence is %d\n", maxLen);
  printf("A possible LIS is: \n");
  printArray(maxLis, maxLen);
  free(lis); free(maxLis);
  return 0;
}

