/* file: med-1.c
* author: David De Potter
* description: minimum edit distance
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

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
  for (int i = 0; i < n; ++i)
    arr[i] = safeCalloc(n, sizeof(int));
  for (int i = 0; i < m; ++i)
    for (int j = 0; j < n; ++j)
      arr[i][j] = INT_MAX; // initialize to infinity
  return arr;
}

char **newPath(int m, int n) {
  /* allocates and initializes a m x n matrix */
  char **arr = safeCalloc(m, sizeof(char *));
  for (int i = 0; i < n; ++i)
    arr[i] = safeCalloc(n, sizeof(char));
  for (int i = 0; i < m; ++i)
    for (int j = 0; j < n; ++j)
      arr[i][j] = ' '; // initialize to empty
  return arr;
}

void freeM(int **m, int n) {
  /* frees a n x n matrix */
  for (int i = 0; i < n; ++i) free(m[i]);
  free(m);
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

void readM(int **arr, int m, int n) {
  /* reads a m x n matrix */
  for (int i = 0; i < m; ++i)
    for (int j = 0; j < n; ++j)
      scanf("%d", &arr[i][j]);
}

void minDist(int **dp, char **path, char *s1, char *s2, int len1, int len2, int costs[]) {
  /* computes the minimum edit distance between s1 and s2 */
  for (int i = 0; i <= len1; ++i) dp[i][0] = i * costs[1];  // empty target string
  for (int j = 0; j <= len2; ++j) dp[0][j] = j * costs[0];  // empty source string

  for (int i = 1; i <= len1; ++i) {
    for (int j = 1; j <= len2; ++j) {
      int cost = INT_MAX;
      if (s1[i - 1] == s2[j - 1]) {
        cost = dp[i - 1][j - 1] + costs[3]; // copy
        path[i][j] = 'c';
        dp[i][j] = cost;
      }
      if (s1[i - 2] == s2[j - 1] && dp[i - 2][j - 2] + costs[4] < cost) {
        cost = dp[i - 2][j - 2] + costs[4];  // twiddle
        path[i][j] = 't';
        dp[i][j] = cost;
      }
      if (s1[i] == s2[j - 1] && dp[i][j - 1] + costs[0] < cost) {
        cost = dp[i][j - 1] + costs[0];  // insert
        path[i][j] = 'i';
        dp[i][j] = cost;
      }
      
      if (s1[i - 1] == s2[j] && dp[i - 1][j] + costs[1] < cost) {
        printf("i = %d, j = %d, s1[i-1] = %c, s2[j] = %c\n", i, j, s1[i-1], s2[j]);
        cost = dp[i - 1][j] + costs[1];  // delete
        path[i][j] = 'd';
        dp[i][j] = cost;
      }

      // perform replace iff it is cheaper than insert and delete
      if (costs[2] < costs[0] + costs[1]) {
        cost = dp[i - 1][j - 1] + costs[2];  // replace
        path[i][j] = 'r';
        dp[i][j] = cost;
      } else { // perform insert and delete
        path[i-1][j] = 'd';
        path[i][j] = 'i';
        dp[i][j] = dp[i-1][j-1] + costs[0] + costs[1];
      }
      
      //if (j == len2 && i < len1 && cost + costs[5] < dp[len1][len2]) {
        //dp[len1][len2] = cost + costs[5];  // kill
        //path[len1][len2] = 'k';
      //}
      //if (i == len1 && j == len2 && cost > dp[i][j]) {
        //path[i][j] = 'k';
      //} else {
        //dp[i][j] = cost;
      //}
    }
  }
}

void reconstruct(int **dp, char** path, char *s1, char *s2, int len1, int len2, char *result, int *resPos, int resLen, int *step) {
  /* reconstructs the minimum edit distance between s1 and s2 and shows how s1 was transformed into s2 using
     result */
  if (len1 == 0 && len2 == 0) return;
  if (path[len1][len2] == 'c') {
    reconstruct(dp, path, s1, s2, len1 - 1, len2 - 1, result, resPos, resLen, step);

    result[(*resPos)++] = s1[len1 - 1];
    printf("%d. Copy %c at index %d: %s\n", (*step)++, s1[len1 - 1], *resPos - 1, result); // copy
    
  } else if (path[len1][len2] == 't') {
    reconstruct(dp, path, s1, s2, len1 - 2, len2 - 2, result, resPos, resLen, step);

    result[(*resPos)++] = s1[len1 - 2];
    result[(*resPos)++] = s1[len1 - 1];
    printf("twiddle %c%c: %s\n", s1[len1 - 2], s1[len1 - 1], result); // twiddle
    
  } else if (path[len1][len2] == 'r') {
    reconstruct(dp, path, s1, s2, len1 - 1, len2 - 1, result, resPos, resLen, step);
    printf("%c %d\n", s1[len1 - 1], len1 - 1);
    printf("%c %d\n", s2[len2 - 1], len2 - 1);
    result[(*resPos)++] = s2[len2 - 1];
    printf("%d. Replace %c with %c at index %d: %s\n", (*step)++, s1[len1 - 1], s2[len2 - 1], *resPos - 1, result); // replace

  } else if (path[len1][len2] == 'd') {
    reconstruct(dp, path, s1, s2, len1 - 1, len2, result, resPos, resLen, step);
    // delete s1[len1 - 1]
    for (int i = *resPos; i < resLen - 1; ++i) {
      result[i] = result[i + 1];
    }
    result[resLen - 1] = '\0';
    printf("%d. Delete %c at index %d: %s\n", (*step)++, s1[len1 - 1], *resPos, result); // delete

  } else if (path[len1][len2] == 'i') {
    reconstruct(dp, path, s1, s2, len1, len2 - 1, result, resPos, resLen, step);
    for (int i = resLen - 1; i > *resPos; --i) {
      result[i] = result[i - 1];
    }
    result[(*resPos)++] = s2[len2 - 1];
    printf("%d. Insert %c at index %d: %s\n", (*step)++, s2[len2 - 1], *resPos - 1, result); // insert
    
  } else if (path[len1][len2] == 'k') {
    reconstruct(dp, path, s1, s2, len1, len2, result, resPos, resLen, step);

    for (int i = *resPos; i < resLen - 1; ++i) {
      result[i] = result[i + 1];
    }
    result[resLen - 1] = '\0';


    printf("%d. Kill: %s\n", (*step)++, result); // kill


  }
}

void printC(char **arr, int m, int n) {
  /* prints a m x n matrix */
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j){
      printf("%c", arr[i][j]);
      if (j < n - 1) printf(" ");
    }
    printf("\n");
  }
}

void freePath (char **path, int m) {
  /* frees a m x n matrix */
  for (int i = 0; i < m; ++i) {
    free(path[i]);
  }
  free(path);
}

int main (int argc, char *argv[]) {
  char *s1 = "algorithm";    // source string
  char *s2 = "altruistic";   // target string
  int len1 = strlen(s1);
  int len2 = strlen(s2);

  int **dp1 = newM(len1+1, len2+1);
  char **path = newPath(len1+1, len2+1);

  int costs[] = {0 ,5, 6, 6, 10, 9};  // costs of insertion, deletion, replacement, copy, twiddle, kill

  minDist(dp1, path, s1, s2, len1, len2, costs);

  printM(dp1, len1+1, len2+1);

  int editDist = dp1[len1][len2];
  printf("Minimum edit distance: %d\n", editDist);

  printC(path, len1+1, len2+1);
  int resLen = MAX(len1, len2) + 1;
  printf("resLen: %d\n", resLen);
  char *result = (char *)malloc(sizeof(char) * (resLen));
  // make copy of s1 and put it in result
  strcpy(result, s1);
  printf("Source string: %s\n", s1);
  int resPos = 0, step = 1;
  reconstruct(dp1, path, s1, s2, len1, len2, result, &resPos, resLen, &step);
  
  freeM(dp1, len1+1);
  freePath(path, len1+1);
  free(result);
  return 0;
}