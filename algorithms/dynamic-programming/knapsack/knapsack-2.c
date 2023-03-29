/* file: knapsack-2.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: 0-1 knapsack problem
     top-down dynamic programming with memoization
     note that we can now use an int function instead
     of a void function, since we use a table from which
     we can retrieve the items that were included in the knapsack
*/ 
 
#include <stdlib.h>
#include <stdio.h>
#define MAX(a, b) ((a) > (b) ? (a) : (b))

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

int **newTable (int n, int m) {
  /* allocates a 2D array of size n x m and initializes it to -1 */
  int **arr = safeCalloc(n, sizeof(int *));
  for (int i = 0; i < n; i++)
    arr[i] = safeCalloc(m, sizeof(int));
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      arr[i][j] = -1;
  return arr;
}

void free2Dmem (int **arr, int n) {
  /* frees the memory allocated for a 2D array of size n x m */
  for (int i = 0; i < n; i++)
    free(arr[i]);
  free(arr);
}

void printItems (int *weights, int *values, int **dp, int n, int w) {
  /* prints the items that were included in the knapsack */
  while (n > 0 && w > 0) {
    if (dp[n][w] != dp[n-1][w]) {
      printf("Item %d: weight = %d, value = %d\n", 
              n+1, weights[n], values[n]);
      w -= weights[n];
    }
    n--;
  }
}

int knapsack (int *weights, int *values, int n, int W, int **dp) {
  /* computes the maximum value that can be put in a knapsack of
     capacity W, given n items with given weights and values
     using top-down dynamic programming with memoization */
  if (n == 0 || W == 0) return 0;
  if (dp[n][W] >= 0) return dp[n][W];
  if (weights[n] > W) { // item n cannot be included
    return dp[n][W] = knapsack(weights, values, n-1, W, dp);
  } else {
    int include = values[n] + knapsack(weights, values, n-1, W-weights[n], dp);
    int exclude = knapsack(weights, values, n-1, W, dp);
    return dp[n][W] = MAX(include, exclude);
  }
}

int main (int argc, char *argv[]) {
  int weights[] = {10, 25, 15, 20, 30, 18, 5, 12, 9, 13};
  int values[] = {120, 90, 80, 200, 280, 180, 50, 20, 100, 250};
  int n = 10;   // number of items
  int W = 60;   // capacity of the knapsack
  int **dp = newTable(n, W+1);
  int maxVal = knapsack(weights, values, n-1, W, dp);
  printf("Maximum value: %d\n", maxVal);
  printf("Items included in the knapsack:\n");
  printItems(weights, values, dp, n-1, W);
  free2Dmem (dp, n);
  return 0;
}



