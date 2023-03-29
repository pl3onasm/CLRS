/* file: mpp-2.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: minimal palindromic partitioning 
                using bottom-up dynamic programming
   time complexity: O(n³)
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int isPalindrome(char *s, int start, int end) {
  /* returns 1 if s[start..end] is a palindrome, 0 otherwise */
  if (start >= end) return 1;
  if (s[start] != s[end]) return 0;
  return isPalindrome(s, start+1, end-1);
}

void printCuts(char *s, int **cuts, int i, int j) {
  /* prints the palindrome partitioning of s[i..j] */
  if (i == j) printf("%c", s[i]);
  else {
    if (cuts[i][j] == j) {
      for (int k = i; k <= j; k++) printf("%c", s[k]);
    } else {
      printCuts(s, cuts, i, cuts[i][j]);
      printf(" | ");
      printCuts(s, cuts, cuts[i][j]+1, j);
    }
  }
}

void partition (char *str, int n, int **dp, int **cuts) {
  /* computes the minimal number of cuts needed to partition str
     into palindromes, and stores the cuts in cuts */
  for (int l = 2; l <= n; l++) {
    for (int i = 0; i < n-l+1; i++) {
      int j = i+l-1;
      if (isPalindrome(str, i, j)) cuts[i][j] = j;
      else {
        dp[i][j] = n;
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
}

int main(int argc, char *argv[]) {
  char str[] = "abbacccabdbacaabccdccddababacdcbbdcaaabbbaaaabdccdeddc"
               "cddeasabeabdccccdeedabbabaaccdeedddceddeccccdaaabccdcc";
  int n = strlen(str);
  int **dp = new2DArray(n, n);
  int **cuts = new2DArray(n, n);
  partition(str, n, dp, cuts);
  printf("The given string is:\n%s\n", str); 
  printf("The minimal number of cuts needed: %d.\n", dp[0][n-1]);
  printf("A minimal palindrome partitioning is:\n");
  printCuts(str, cuts, 0, n-1); 
  printf("\n");
  free2DArray(dp, n);
  free2DArray(cuts, n);
  return 0;
}
