/* file: med-1.c
* author: David De Potter
* description: minimum edit distance
*   bottom-up dynamic programming (with path reconstruction)  
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define MIN(a,b) ((a) < (b) ? (a) : (b))

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

int **newM(int m, int n) {
  /* allocates and initializes a m x n matrix */
  int **arr = safeCalloc(m, sizeof(int *));
  for (int i = 0; i < m; ++i)
    arr[i] = safeCalloc(n, sizeof(int));
  return arr;
}

void freeM(int **m, int n) {
  /* frees a n x n matrix */
  for (int i = 0; i < n; ++i) free(m[i]);
  free(m);
}

void med(int **dp, char *s1, char *s2, int len1, int len2, int *costs) {
  /* bottom-up approach to get the minimum edit distance between s1[0..i] and s2[0..j] */
  dp[len1][len2] = INT_MAX;                  

  for (int i = 0; i <= len1; ++i)    // empty target string, so                   
    dp[i][0] = costs[1] * i;         // delete all chars of source

  for (int j = 0; j <= len2; ++j)    // empty source string, so 
    dp[0][j] = j * costs[0];         // insert all chars of target

  for (int i = 1; i <= len1; ++i){
    for (int j = 1; j <= len2; ++j){
      int cost = INT_MAX; 

      if (s1[i-1] == s2[j-1])
        cost = MIN(costs[3] + dp[i-1][j-1], cost);    // copy

      if (i > 1 && j > 1 && s1[i-1] == s2[j-2] && s1[i-2] == s2[j-1])
        cost = MIN(costs[4] + dp[i-2][j-2], cost);    // twiddle

      cost = MIN(costs[0] + dp[i][j-1], cost);        // insert
      cost = MIN(costs[1] + dp[i-1][j], cost);        // delete
      cost = MIN(costs[2] + dp[i-1][j-1], cost);      // replace

      if (j == len2 &&  i < len1)  
        // update the cost of killing the remaining source chars
        dp[len1][len2] = MIN(costs[5] + dp[i-1][len2], dp[len1][len2]);

      dp[i][j] = (i == len1 && j == len2) ? MIN(cost, dp[i][j]) : cost;
    }
  }
}

void printPath(int **dp, char *s1, char *s2, int len1, int len2, int *costs) {
  /* recursively prints the path showing the transformations from s1 to s2 */
  if (dp[len1][len2] == 0) return;

  int cost = dp[len1][len2];
  
  if (len1 == strlen(s1) && len2 == strlen(s2)) 
    // check if a kill operation was performed
    for (int i = len1-1; i >= 0; --i) {
      if (dp[i][len2] + costs[5] == dp[len1][len2]){
        printPath (dp, s1, s2, i, len2, costs);
        printf("kill %d chars from %d to %d\n", len1 - i, i, len1-1);
        return; 
      }
    }

  if (len1 > 0 && len2 > 0 &&
    s1[len1-1] == s2[len2-1] && cost == costs[3] + dp[len1-1][len2-1]){
    printPath(dp, s1, s2, len1-1, len2-1, costs);
    printf("copy %c at %d\n", s1[len1-1], len1-1);

  } else if (len1 > 1 && len2 > 1 && s1[len1-1] == s2[len2-2] 
      && s1[len1-2] == s2[len2-1] && cost == costs[4] + dp[len1-2][len2-2]){
    printPath(dp, s1, s2, len1-2, len2-2, costs);
    printf("twiddle %c and %c at %d and %d\n", s1[len1-1], s1[len1-2], len1-2, len1-1);

  } else if (len2 > 0 && cost == costs[0] + dp[len1][len2-1]){
    printPath(dp, s1, s2, len1, len2-1, costs);
    printf("insert %c at %d\n", s2[len2-1], len1);

  } else if (len1 > 0 && cost == costs[1] + dp[len1-1][len2]){
    printPath(dp, s1, s2, len1-1, len2, costs);
    printf("delete %c at %d\n", s1[len1-1], len1-1);

  } else if (len1 > 0 && len2 > 0 && cost == costs[2] + dp[len1-1][len2-1]){
    printPath(dp, s1, s2, len1-1, len2-1, costs);
    printf("replace %c with %c at %d\n", s1[len1-1], s2[len2-1], len1-1);
  }
}

void printM(int **arr, int m, int n) {
  /* prints a m x n matrix */
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j){
      printf("%d", arr[i][j]);
      if (j < n - 1) printf(" ");
    }
    printf("\n");
  }
}

int main (int argc, char *argv[]) {
  char *s1 = "";    // source string
  char *s2 = "sitting";   // target string
  int len1 = strlen(s1);
  int len2 = strlen(s2);

  int **dp1 = newM(len1+1, len2+1);

  // costs for insert, delete, replace, copy, twiddle, kill
  int costs[] = {2 ,3, 7, 2, 3, 6};  

  med(dp1, s1, s2, len1, len2, costs);

  printM(dp1, len1+1, len2+1);

  printf("Minimum edit distance: %d\n", dp1[len1][len2]);

  printPath(dp1, s1, s2, len1, len2, costs);

  freeM(dp1, len1+1);
  
  return 0;
}