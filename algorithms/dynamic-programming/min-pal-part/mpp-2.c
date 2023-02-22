/* file: mpp-2.c
   author: David De Potter
   description: minimal palindromic partitioning 
                using bottom-up dynamic programming
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b));

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
  int **a = safeCalloc(n, sizeof(int *));
  for (int i = 0; i < n; i++) 
    a[i] = safeCalloc(m, sizeof(int));
  return a;
}

void free2DArray(int **a, int n) {
  for (int i = 0; i < n; i++) free(a[i]);
  free(a);
}

void print2DArray(int **a, int n, int m) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      printf("%d ", a[i][j]);
    }
    printf("\n");
  }
}

int isPalindrome(char *s, int start, int end) {
  if (start >= end) return 1;
  if (s[start] != s[end]) return 0;
  return isPalindrome(s, start+1, end-1);
}

void printCuts(char *s, int **cuts, int i, int j) {
  if (i == j) {
    printf("%c", s[i]);
    return;
  } else {
    if (cuts[i][j] == j) {
      for (int k = i; k <= j; k++) printf("%c", s[k]);
      return;
    } else {
      printCuts(s, cuts, i, cuts[i][j]);
      printf(" | ");
      printCuts(s, cuts, cuts[i][j]+1, j);
    }
  }
}

int main(int argc, char *argv[]) {
  char str[] = "abbacccabdbacaabccdccddababacdcbbdcaaabbbaaaabdccdeddc"
                "ddeasabeabdccccdeedabbabaaccdeedddceddeccccdaaabccdcc";
  int n = strlen(str);
  int **dp = new2DArray(n, n);
  int **cuts = new2DArray(n, n);
  for (int i = 0; i < n; i++) 
    for (int j = 0; j < n; j++) 
      dp[i][j] = 0;

  for (int l = 2; l <= n; l++) {
    for (int i = 0; i < n-l+1; i++) {
      int j = i+l-1;
      if (isPalindrome(str, i, j)) {
        dp[i][j] = 0;
        cuts[i][j] = j;
      } else {
        dp[i][j] = INT_MAX;
        for (int k = i; k < j; k++) {
          int q = dp[i][k] + dp[k+1][j] + 1;
          if (q < dp[i][j]){
            dp[i][j] = q;
            cuts[i][j] = k;
          }
        }
      }
    }
  }

  printf("The minimal number of cuts needed is %d.\n", dp[0][n-1]);
  printf("A palindrome partitioning is: ");
  printCuts(str, cuts, 0, n-1); 
  printf("\n");
  free2DArray(dp, n);
  free2DArray(cuts, n);
  return 0;
}
