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

void freeM(int **arr, int m) {
  /* frees a m x n matrix */
  for (int i = 0; i < m; ++i) free(arr[i]);
  free(arr);
}

void med(int **dp, char *s1, char *s2, int len1, int len2, int *costs) {
  /* bottom-up approach to get the minimum edit distance between s1[0..i] and s2[0..j] */
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
    for (int i = len1-1; i >= 0; --i) {
      if (dp[i][len2] + costs[5] == dp[len1][len2]){
        printPath (dp, s1, s2, i, len2, costs, result, resLen, resIdx);
        *resLen = *resLen - (len1 - i);
        result[*resLen] = '\0';
        printf("  - Kill %d:\t\t", len1 - i);
        print(result, *resIdx, *resLen);
        return; 
      }
    }

  // copy
  if (len1 > 0 && len2 > 0 && s1[len1-1] == s2[len2-1] 
      && cost == costs[0] + dp[len1-1][len2-1]){
    printPath(dp, s1, s2, len1-1, len2-1, costs, result, resLen, resIdx);
    (*resIdx)++; 
    printf("  - Copy %c:\t\t", s1[len1-1]);
    print(result, *resIdx, *resLen);

  // twiddle
  } else if (len1 > 1 && len2 > 1 && s1[len1-1] == s2[len2-2] 
      && s1[len1-2] == s2[len2-1] && cost == costs[4] + dp[len1-2][len2-2]){
    printPath(dp, s1, s2, len1-2, len2-2, costs, result, resLen, resIdx);
    result[(*resIdx)++] = s1[len1-1];
    result[(*resIdx)++] = s1[len1-2];
    printf("  - Twiddle %c and %c:\t", s1[len1-1], s1[len1-2]);
    print (result, *resIdx, *resLen);

  // insertion
  } else if (len2 > 0 && cost == costs[1] + dp[len1][len2-1]){
    printPath(dp, s1, s2, len1, len2-1, costs, result, resLen, resIdx);
    (*resLen)++; 
    for (int i = *resLen; i >= *resIdx; --i) result[i+1] = result[i];
    result[(*resIdx)++] = s2[len2-1];
    result[*resLen] = '\0';
    printf("  - Insert %c:\t\t", s2[len2-1]);
    print(result, *resIdx, *resLen);

  // deletion
  } else if (len1 > 0 && cost == costs[2] + dp[len1-1][len2]){
    printPath(dp, s1, s2, len1-1, len2, costs, result, resLen, resIdx);
    for (int i = *resIdx; i < *resLen; ++i) result[i] = result[i+1];
    (*resLen)--;
    printf("  - Delete %c:\t\t", s1[len1-1]);
    print(result, *resIdx, *resLen);

  // replacement
  } else if (len1 > 0 && len2 > 0 && cost == costs[3] + dp[len1-1][len2-1]){
    printPath(dp, s1, s2, len1-1, len2-1, costs, result, resLen, resIdx);
    result[*resIdx] = s2[*resIdx]; 
    (*resIdx)++;
    printf("  - Replace %c with %c:\t", s1[len1-1], s2[len2-1]); 
    print(result, *resIdx, *resLen);
  }
}

void printResult(int **dp, char *s1, char *s2, int len1, int len2, int *costs) {
  /* prints the minimum edit distance and the path */
  printf("Task: source -> target\n      ");
  printf("'%s' -> '%s'\n\n", s1, s2);
  printf("Costs: copy = %d, insert = %d, delete = %d,\n       replace = %d, twiddle = %d"
    ", kill = %d\n\n", costs[0], costs[1], costs[2], costs[3], costs[4], costs[5]); 
  printf("Edit sequence:\n  - Source:\t\t'%s'\n", s1);
  int resIdx = 0, resLen = len1;
  char *result = (char *) malloc(sizeof(char) * (len1 + len2 + 2));
  strcpy(result, s1); 
  printPath(dp, s1, s2, len1, len2, costs, result, &resLen, &resIdx);
  printf("  - Result:\t\t'%s'\n\n", s2);
  printf("Minimum edit distance: %d\n", dp[len1][len2]);
  free(result);
}

char *readString(){
  /* reads a string of arbitrary length from stdin */
  char n; int index = 0, cap = 128;
  char *arr = calloc(cap,sizeof(char)); 
  while (scanf("%c", &n) && n != '\n' && n != EOF){
    index++;
    if (index % cap == 0) {
      arr = realloc(arr, 2*index*sizeof(char));
      cap *= 2;
    }
    arr[index-1] = n;
  }
  arr[index] = '\0';
  return arr; 
}

int *readCosts() {
  /* reads the costs for the operations from stdin */
  int *costs = (int *) malloc(sizeof(int) * 6);
  for (int i = 0; i < 6; ++i) scanf("%d ", &costs[i]);
  return costs;
}

int main (int argc, char *argv[]) {
  char *s1 = readString();    // source string
  char *s2 = readString();    // target string
  
  // operation costs for copy, insert, 
  // delete, replace, twiddle, kill
  int *costs = readCosts();   
  
  int len1 = strlen(s1);
  int len2 = strlen(s2);
  int **dp1 = newM(len1+1, len2+1);

  med(dp1, s1, s2, len1, len2, costs);

  printResult(dp1, s1, s2, len1, len2, costs);

  freeM(dp1, len1+1);
  free(s1); free(s2);
  free(costs);
  return 0;
}