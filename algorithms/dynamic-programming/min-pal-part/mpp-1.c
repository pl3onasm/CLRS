/* file: mpp-1.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: minimal palindromic partitioning 
                using recursion with backtracking
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void printArray(int *a, int n) {
  for (int i = 0; i < n; i++) printf("%d ", a[i]);
  printf("\n");
}

int isPalindrome(char *s, int start, int end) {
  if (start >= end) return 1;
  if (s[start] != s[end]) return 0;
  return isPalindrome(s, start+1, end-1);
}

void copyArray(int *copy, int *source, int n) {
  for (int i = 0; i < n; i++) copy[i] = source[i];
}

void partition(int left, int right, char *a, int *cuts, 
  int *minCuts, int *min, int *count) {
  if (left > right) {
    if (*count < *min) {
      *min = *count;
      copyArray(minCuts, cuts, right);
    }
    return;
  }
  for (int j = left; j <= right; j++) {
    if (isPalindrome(a, left, j)) {
      ++*count;     
      ++cuts[j];  // mark the cut
      partition(j+1, right, a, cuts, minCuts, min, count);
      --*count;	  // backtrack
      --cuts[j];  // unmark the cut
    }
  }
}

int main(int argc, char *argv[]) {
  char str[] = "ababbbabbabababababbbabbaaabbababbbbaaaab";
  int cuts[50]={0}, minCuts[50]={0};
  int min = INT_MAX, count = 0;
  partition(0, strlen(str)-1, str, cuts, minCuts, &min, &count);
  printf("The minimal number of cuts is %d.\n", min-1);
  printf("A minimal partitioning is: ");
  for (int i = 0; i < strlen(str); i++){
    printf("%c", str[i]);
    if (minCuts[i] && i < strlen(str)-1) printf(" | ");
  }
  printf("\n");
  return 0;
}