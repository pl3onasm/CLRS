/* file: med-1.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: levenstein distance
     bottom-up dynamic programming with path reconstruction 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define INF INT_MAX

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

void med(int **dp, char *s1, char *s2, int len1, int len2) {
  /* bottom-up approach to get the levenshtein distance between s1 and s2 */
  
  for (int i = 0; i <= len1; ++i)    // empty target string, so                   
    dp[i][0] = i;           	       // delete all chars of source

  for (int j = 0; j <= len2; ++j)    // empty source string, so 
    dp[0][j] = j;                    // insert all chars of target

  for (int i = 1; i <= len1; ++i){
    for (int j = 1; j <= len2; ++j){

      // if the last chars are equal, no operation is needed
      if (s1[i-1] == s2[j-1]) dp[i][j] = dp[i-1][j-1];
      
      // if the last chars are different, we need to find the minimum
      // of the costs of the three possible operations
      else dp[i][j] = 1 + MIN(dp[i-1][j-1], MIN(dp[i-1][j], dp[i][j-1]));
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
  char *result, int *resLen, int *resIdx) {
  /* recursively prints the path showing the transformations from s1 to s2 */
  
  if (len1 == 0 && len2 == 0 && dp[len1][len2] == 0) return;

  int cost = dp[len1][len2];

  // copy
  if (len1 > 0 && len2 > 0 && s1[len1-1] == s2[len2-1] 
      && cost == dp[len1-1][len2-1]){
    printPath(dp, s1, s2, len1-1, len2-1, result, resLen, resIdx);
    ++*resIdx; 
    printf("  - Copy %c:           ", s1[len1-1]);

  // insertion
  } else if (len2 > 0 && cost == 1 + dp[len1][len2-1]){
    printPath(dp, s1, s2, len1, len2-1, result, resLen, resIdx);
    for (int i = ++*resLen; i >= *resIdx; --i) result[i+1] = result[i];
    result[(*resIdx)++] = s2[len2-1];
    printf("  - Insert %c:         ", s2[len2-1]);

  // deletion
  } else if (len1 > 0 && cost == 1 + dp[len1-1][len2]){
    printPath(dp, s1, s2, len1-1, len2, result, resLen, resIdx);
    for (int i = *resIdx; i < *resLen; ++i) result[i] = result[i+1];
    --*resLen;
    printf("  - Delete %c:         ", s1[len1-1]);

  // replacement/substitution
  } else if (len1 > 0 && len2 > 0 && cost == 1 + dp[len1-1][len2-1]){
    printPath(dp, s1, s2, len1-1, len2-1, result, resLen, resIdx);
    result[*resIdx] = s2[*resIdx]; 
    ++*resIdx;
    printf("  - Replace %c -> %c:   ", s1[len1-1], s2[len2-1]); 
  }

  print(result, *resIdx, *resLen);
}

void printResult(int **dp, char *s1, char *s2, int len1, int len2) {
  /* prints the minimum edit distance and the path */
  int resIdx = 0, resLen = len1;
  char *result = safeCalloc(len1 + len2 + 2, sizeof(char));
  strcpy(result, s1); 
  printf("Task: source -> target\n");
  printf("Source: %s\n", s1);
  printf("Target: %s\n\n", s2);
  printf("Edit sequence:\n  - Source:           %s\n", s1);
  printPath(dp, s1, s2, len1, len2, result, &resLen, &resIdx);
  printf("  - Result:           %s\n\n", result);
  printf("Levenshtein distance: %d\n", dp[len1][len2]);
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

int main (int argc, char *argv[]) {
  int len1, len2;  // length of source and target string
  char *s1 = readString(&len1);    // source string
  char *s2 = readString(&len2);    // target string

  int **dp = newM(len1+1, len2+1);

  med(dp, s1, s2, len1, len2);
  printResult(dp, s1, s2, len1, len2);

  freeM(dp, len1+1);
  free(s1); free(s2);
  return 0;
}