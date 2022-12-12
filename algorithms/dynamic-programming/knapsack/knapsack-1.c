/* file: knapsack-1.c
* author: David De Potter
* description: 0-1 knapsack problem
* naive recursive implementation
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

void printArray (int *arr, int n) {
  // prints an array of size n
  printf("[");
  for (int i = 0; i < n; i++) {
    printf("%d", (arr)[i]);
    if (i < n-1) printf(", ");
  }
  printf("]\n");
}

void printItems (int *weights, int *values, int *taken, int n) {
  /* prints the items that were taken */
  printf("The following items were taken: \n");
  for (int i = 0; i < n; i++) {
    if (taken[i])
      printf("item %d: weight = %d kg, value = € %d\n", 
              i+1, weights[i], values[i]);
  }
}

void knapsack (int *weights, int *values, int *taken, int n, int idx, 
int W, int val, int *maxVal, int *maxTaken) {
  /* computes the maximum value that can be put in a knapsack of
     capacity W, given n items with given weights and values; 
     also stores the items that were chosen to get this max value */
  if (idx == -1) {
    if (val > *maxVal) {
      *maxVal = val;
      for (int i = 0; i < n; i++)  // store the items that were taken
        maxTaken[i] = taken[i];
    }
    return;
  }
  if (weights[idx] <= W) {
    taken[idx] = 1;   // take the item
    knapsack(weights, values, taken, n, idx-1, W-weights[idx], 
             val+values[idx], maxVal, maxTaken);
  }
  taken[idx] = 0;     // don't take the item
  knapsack(weights, values, taken, n, idx-1, W, val, maxVal, maxTaken);
}


int main (int argc, char *argv[]) {
  int weights[] = {10, 25, 15, 20, 30, 18, 5, 12, 9, 13};
  int values[] = {120, 90, 80, 200, 280, 180, 50, 20, 100, 250};
  int n = 10;   // number of items
  int W = 60;   // capacity of the knapsack
  int *taken = safeCalloc (n, sizeof(int));
  int *maxTaken = safeCalloc (n, sizeof(int));
  int maxVal = 0;
  knapsack(weights, values, taken, n, n-1, W, 0, &maxVal, maxTaken);
  printf("Maximum value: %d\n", maxVal);
  printItems(weights, values, maxTaken, n);
  free(taken); free(maxTaken);
  return 0;
}



