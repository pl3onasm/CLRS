/* file: lis-3.c
* author: David De Potter
* description: longest increasing subsequence
* bottom-up dynamic programming, optimized
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

int *createTable (int n) {
  /* creates a table of size n */
  int *table = safeCalloc(n, sizeof(int));
  for (int i = 0; i < n; i++) 
    table[i] = 1;
  return table;
}

int computeTable(int *arr, int n, int *table) {
  /* computes the table and returns the index of 
     the last element of the LIS */
  int indexMax, max = 0;
  for (int i = 1; i < n; i++) {
    for (int j = 0; j < i; j++) {
      if (arr[j] < arr[i] && table[j] >= table[i]) {
        table[i] = table[j] + 1;
        if (table[i] > max) {
          max = table[i];
          indexMax = i;
        }
      }
    }
  }
  return indexMax;
}

void reconstructLIS(int *arr, int *table, int n, int indexMax, int *lis) {
  // reconstructs the LIS 
  int i = table[indexMax]-1;
  lis[i] = arr[indexMax];
  for (int j = indexMax-1; j >= 0; j--) {
    if (table[j] == table[indexMax]-1) {
      lis[--i] = arr[j];
      indexMax = j;
    }
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
  int *table = createTable(n); 
  int indexMax = computeTable(arr, n, table);
  int *lis = safeCalloc(n, sizeof(int));
  reconstructLIS(arr, table, n, indexMax, lis);
  int lisLen = table[indexMax];
  printf("The given array is:\n");
  printArray(arr, n);
  printf("The length of the longest increasing subsequence is %d.\n", lisLen);
  printf("A possible LIS is:\n");
  printArray(lis, lisLen);
  free(lis); free(table);

  return 0;
}