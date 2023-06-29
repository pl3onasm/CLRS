/* file: asp-1.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: activity selection problem (ASP)
                using bottom-up dynamic programming
   assumptions: the activities are sorted in increasing order of finish time
   time complexity: O(n³)
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define INF INT_MAX

typedef struct {
  int start, finish;
} act;

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

int **new2DArray(int n, int m) {
  /* allocates a 2D array of size n x m */
  int **a = safeCalloc(n, sizeof(int *));
  for (int i = 0; i < n; i++) 
    a[i] = safeCalloc(m, sizeof(int));
  return a;
}

void free2DArray(int **a, int n) {
  /* frees a 2D array of n rows */
  for (int i = 0; i < n; i++) free(a[i]);
  free(a);
}

void printActs(act *acts, int **maxSet, int i, int j) {
  /* prints the selected activities in [i..j] */
  if (maxSet[i][j] > 0) {
    int k = maxSet[i][j];
    printActs(acts, maxSet, i, k);
    printf("Activity %d: [%d, %d)\n", k, acts[k].start, acts[k].finish);
    printActs(acts, maxSet, k, j);
  }
}

void selectActs(act *acts, int n, int **maxSet, int **dp) {
  for (int l = 1; l < n; l++) {  // l = length of interval
    for (int i = 0; i < n-l+1; i++) {
      int j = i+l;
      if (acts[i].finish <= acts[j].start) {  // non-overlapping intervals
        for (int k = i+1; k < j; k++) {       // find the optimal k
          if (acts[k].start >= acts[i].finish && acts[k].finish <= acts[j].start) {
            int num = 1 + dp[i][k] + dp[k][j];  // number of activities in [i..j]
            if (num > dp[i][j]) {   // update dp[i][j] if necessary
              dp[i][j] = num;
              maxSet[i][j] = k;
            }
          }
        }
      }
    }
  }
}

int main (int argc, char *argv[]) {
  act acts[] = {{0, 0}, {1, 4}, {3, 5}, {0, 6}, {5, 7}, {3, 9}, 
      {5, 9}, {6, 10}, {8, 11}, {8, 12}, {2, 14}, {12, 16}, {INF, INF}};
  int n = 11+2;   // number of activities + 2 sentinel activities
  int **dp = new2DArray(n, n); // dp[i][j] = max number of activities in [i..j]
  int **maxSet = new2DArray(n, n); // stores the index of the selected activity
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) 
      maxSet[i][j] = -1;
  selectActs (acts, n, maxSet, dp);
  printf("The maximal number of activities is %d.\n", dp[0][n-1]);
  printf("The selected activities are:\n");
  printActs(acts, maxSet, 0, n-1);
  free2DArray(dp, n);
  free2DArray(maxSet, n);
  return 0;
}