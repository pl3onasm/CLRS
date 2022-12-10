/* file: lps-2.c
* author: David De Potter
* description: longest palindromic subsequence
* bottom-up dynamic programming implementation
*/ 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX(a,b) ((a)>(b)?(a):(b));

void *safeCalloc (int n, int size) {
  /* allocates n elements of size size, initializes them to 0, and
     checks whether the allocation was successful */
  void *ptr = calloc(n, size);
  if (ptr == NULL) {
    printf("Error: calloc(%d, %d) failed. Out of memory?\n", n, size);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

int **createTable (int n, int m) {
  /* creates a table of size n x m */
  int **table = safeCalloc(n, sizeof(int *));
  for (int i = 0; i < n; i++) 
    table[i] = safeCalloc(m, sizeof(int));
  return table;
}

char *reverse (char *s) {
  /* returns a reversed copy of s */
  int n = strlen(s);
  char *rev = safeCalloc(n+1, sizeof(char));
  for (int i = 0; i < n; i++)
    rev[i] = s[n-1-i];
  return rev;
}

int fillTable (int **table, const char *a, const char *rev, int la){
  /* fills the table with the lengths of the lcs's of the prefixes of a  
     and its reverse rev, and returns the length of the lps */
  for (int i=1; i<=la; ++i)     // fill table in row-major order
    for (int j=1; j<=la; ++j){
      if (a[i-1] == rev[j-1]) table[i][j] = table[i-1][j-1] + 1; 
      else table[i][j] = MAX(table[i-1][j],table[i][j-1]); 
    }
  return table[la][la];
} 

void reconstructLps (int **table, char *a, int x, int y, char *lps, int z){
  /* fills the string lps in reverse order (index z starts at the end) 
     with the longest palindromic subsequence */
  if (x == 0 || y == 0) return;
  if (table[x][y] == table[x-1][y]) 
    reconstructLps(table, a, x-1, y, lps, z);
  else if (table[x][y] == table[x][y-1]) 
    reconstructLps(table, a, x, y-1, lps, z);
  else {
    lps[z] = a[x-1]; 
    reconstructLps(table, a, x-1, y-1, lps, z-1);
  }
} 

void free2Dmem (int **arr, int m) {
  for (int row = 0; row < m; ++row) free(arr[row]);
  free(arr);
}

int main (int argc, char *argv[]) {
  char *a = "ktkhilsistpqrmvethtyfuwngkny-yndiinbudofcytptzehrpswihestorbilhtars"; 
  char *rev = reverse(a);
  int la = strlen(a);
  int **table = createTable(la+1, la+1); 
  int lpslen = fillTable(table, a, rev, la); 
  char *lps = safeCalloc(lpslen+1, sizeof(char));
  reconstructLps(table, a, la, la, lps, lpslen-1);
  lps[lpslen] = '\0';   
  printf("Given string:\n%s\n", a);
  printf("The length of the LPS is %d.\n", lpslen);
  printf("A possibly non-unique LPS is %s.\n", lps);
  free(lps); free(rev); free2Dmem(table, la+1);
  return 0;
}

