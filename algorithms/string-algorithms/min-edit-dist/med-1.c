/* file: med-1.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: minimum edit distance
     bottom-up dynamic programming with path reconstruction 
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

void *safeRealloc (void *ptr, int newSize) {
  // reallocates memory and checks whether the allocation was successful
  ptr = realloc(ptr, newSize);
  if (ptr == NULL) {
    printf("Error: realloc(%d) failed. Out of memory?\n", newSize);
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

void freeM(int **arr, int m) {
  /* frees a m x n matrix */
  for (int i = 0; i < m; ++i) free(arr[i]);
  free(arr);
}

void med(int **dp, char *s1, char *s2, int len1, int len2, int *costs) {
  /* bottom-up approach to get the minimum edit distance between s1 and s2 */
  dp[len1][len2] = INT_MAX;                  

  for (int i = 0; i <= len1; ++i)    // empty target string, so                   
    dp[i][0] = costs[2] * i;         // delete all chars of source

  for (int j = 0; j <= len2; ++j)    // empty source string, so 
    dp[0][j] = j * costs[1];         // insert all chars of target

  for (int i = 1; i <= len1; ++i){
    for (int j = 1; j <= len2; ++j){
      int cost = INT_MAX; 

      if (s1[i-1] == s2[j-1])
        cost = MIN(costs[0] + dp[i-1][j-1], cost);    // copy

      if (i > 1 && j > 1 && s1[i-1] == s2[j-2] && s1[i-2] == s2[j-1])
        cost = MIN(costs[4] + dp[i-2][j-2], cost);    // twiddle

      cost = MIN(costs[1] + dp[i][j-1], cost);        // insert
      cost = MIN(costs[2] + dp[i-1][j], cost);        // delete
      cost = MIN(costs[3] + dp[i-1][j-1], cost);      // replace

      if (j == len2 &&  i < len1)  
        // update the cost of killing the remaining source chars
        dp[len1][len2] = MIN(costs[5] + dp[i-1][len2], dp[len1][len2]);

      dp[i][j] = (i == len1 && j == len2) ? MIN(cost, dp[i][j]) : cost;
    }
  }
}

void print(char *result, int idx, int len) {
  /* prints the temporary result string */
  for (int i = 0; i < idx; ++i) printf("%c", result[i]);
  printf("-");
  for (int i = idx; i < len; ++i) printf("%c", result[i]);
  printf("\n");
}

void printPath(int **dp, char *s1, char *s2, int len1, int len2, 
  int *costs, char *result, int *resLen, int *resIdx) {
  /* recursively prints the path showing the transformations from s1 to s2 */
  
  if (len1 == 0 && len2 == 0 && dp[len1][len2] == 0) return;

  int cost = dp[len1][len2];
 
  // kill
  if (len1 == strlen(s1) && len2 == strlen(s2)) 
    // check if a kill operation was performed
    for (int i = len1-1; i >= 0; --i) 
      if (dp[i][len2] + costs[5] == dp[len1][len2]){
        printPath (dp, s1, s2, i, len2, costs, result, resLen, resIdx);
        *resLen -= len1 - i;
        result[*resLen] = '\0';
        printf("  - Kill %02d chars:    ", len1 - i); 
        break; 
      }

  // copy
  if (len1 > 0 && len2 > 0 && s1[len1-1] == s2[len2-1] 
      && cost == costs[0] + dp[len1-1][len2-1]){
    printPath(dp, s1, s2, len1-1, len2-1, costs, result, resLen, resIdx);
    ++*resIdx; 
    printf("  - Copy %c:           ", s1[len1-1]);

  // twiddle
  } else if (len1 > 1 && len2 > 1 && s1[len1-1] == s2[len2-2] 
      && s1[len1-2] == s2[len2-1] && cost == costs[4] + dp[len1-2][len2-2]){
    printPath(dp, s1, s2, len1-2, len2-2, costs, result, resLen, resIdx);
    result[(*resIdx)++] = s1[len1-1];
    result[(*resIdx)++] = s1[len1-2];
    printf("  - Twiddle %c and %c:  ", s1[len1-1], s1[len1-2]);

  // insertion
  } else if (len2 > 0 && cost == costs[1] + dp[len1][len2-1]){
    printPath(dp, s1, s2, len1, len2-1, costs, result, resLen, resIdx);
    for (int i = ++*resLen; i >= *resIdx; --i) result[i+1] = result[i];
    result[(*resIdx)++] = s2[len2-1];
    printf("  - Insert %c:         ", s2[len2-1]);

  // deletion
  } else if (len1 > 0 && cost == costs[2] + dp[len1-1][len2]){
    printPath(dp, s1, s2, len1-1, len2, costs, result, resLen, resIdx);
    for (int i = *resIdx; i < *resLen; ++i) result[i] = result[i+1];
    --*resLen;
    printf("  - Delete %c:         ", s1[len1-1]);

  // replacement/substitution
  } else if (len1 > 0 && len2 > 0 && cost == costs[3] + dp[len1-1][len2-1]){
    printPath(dp, s1, s2, len1-1, len2-1, costs, result, resLen, resIdx);
    result[*resIdx] = s2[*resIdx]; 
    ++*resIdx;
    printf("  - Replace %c -> %c:   ", s1[len1-1], s2[len2-1]); 
  }

  print(result, *resIdx, *resLen);
}

void printResult(int **dp, char *s1, char *s2, int len1, int len2, int *costs) {
  /* prints the minimum edit distance and the path */
  int resIdx = 0, resLen = len1;
  char *result = safeCalloc(len1 + len2 + 2, sizeof(char));
  strcpy(result, s1); 

  printf("Task: source -> target\n      ");
  printf("'%s' -> '%s'\n\n", s1, s2);
  printf("Costs: copy = %d, insert = %d, delete = %d,\n       "
         "replace = %d, twiddle = %d, kill = %d\n\n", 
         costs[0], costs[1], costs[2], costs[3], costs[4], costs[5]); 
  printf("Edit sequence:\n  - Source:           '%s'\n", s1);
  printPath(dp, s1, s2, len1, len2, costs, result, &resLen, &resIdx);
  printf("  - Result:           '%s'\n\n", result);
  printf("Minimum edit distance: %d\n", dp[len1][len2]);
  free(result);
}

char *readString(int *size) {
  /* reads a string of arbitrary length from stdin */
  char c; int len = 0; 
  char *str = safeCalloc(100, sizeof(char));
  while (scanf("%c", &c) == 1 && c != '\n') {
    str[len++] = c; 
    if (len % 100 == 0) str = safeRealloc(str, (len+100) * sizeof(char));
  }
  str[len] = '\0';
  *size = len;
  return str;
}

int *readCosts() {
  /* reads the costs for the operations from stdin */
  int *costs = safeCalloc(6, sizeof(int));
  for (int i = 0; i < 6; ++i) scanf("%d ", &costs[i]);
  return costs;
}

int main (int argc, char *argv[]) {
  int len1, len2;  // length of source and target string
  char *s1 = readString(&len1);    // source string
  char *s2 = readString(&len2);    // target string

  // operation costs for copy, insert, 
  // delete, replace, twiddle, kill
  int *costs = readCosts();   
  int **dp1 = newM(len1+1, len2+1);

  med(dp1, s1, s2, len1, len2, costs);
  printResult(dp1, s1, s2, len1, len2, costs);

  freeM(dp1, len1+1);
  free(s1); free(s2);
  free(costs);
  return 0;
}