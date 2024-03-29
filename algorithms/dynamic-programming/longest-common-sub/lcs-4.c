/* file: lcs-4.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: longest common subsequence
     bottom-up dynamic programming implementation
     Notice that we can now work with sequences that  
     are much longer than in the naive recursive 
     implementation, thanks to the O(mn) running time.
*/ 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX(a,b) ((a)>(b)?(a):(b));

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

int **createTable (int n, int m) {
  /* creates a table of size n x m */
  int **table = safeCalloc(n, sizeof(int *));
  for (int i = 0; i < n; i++) 
    table[i] = safeCalloc(m, sizeof(int));
  return table;
}

int fillTable (int **table, const char *a, const char *b, int la, int lb){
  /* fills the table with the lengths of the lcs's of the prefixes 
      of a and b and returns the length of the lcs of a and b */
  for (int i=1; i<=la; ++i)     // fill table in row-major order
    for (int j=1; j<=lb; ++j){
      if (a[i-1] == b[j-1]) table[i][j] = table[i-1][j-1] + 1; 
      else table[i][j] = MAX(table[i-1][j],table[i][j-1]); 
    }
  return table[la][lb];
} 

void reconstructLcs (int **table, char *a, int x, int y, char *lcs, int z){
  /* fills the string lcs in reverse order (index z starts at the end) 
     with the longest common subsequence */
  if (x == 0 || y == 0) return;
  if (table[x][y] == table[x-1][y]) 
    reconstructLcs(table, a, x-1, y, lcs, z);
  else if (table[x][y] == table[x][y-1]) 
    reconstructLcs(table, a, x, y-1, lcs, z);
  else {
    lcs[z] = a[x-1]; 
    reconstructLcs(table, a, x-1, y-1, lcs, z-1);
  }
} 

void free2Dmem (int** arr, int m) {
  for (int row = 0; row < m; ++row) free(arr[row]);
  free(arr);
}

int main (int argc, char *argv[]) {
  char *a = "ACCGATATCAGGATCGGATACGGTGAGCTAACATGATCGGGGTAACG", 
       *b = "GTAGTAATTACGAACATATTGATCCTACCCTATCCCAATGCATT";
  int la = strlen(a), lb = strlen(b);
  int **table = createTable(la+1, lb+1); 
  int lcslen = fillTable(table, a, b, la, lb); 
  char *lcs = safeCalloc(lcslen+1, sizeof(char));
  reconstructLcs(table, a, la, lb, lcs, lcslen-1);
  lcs[lcslen] = '\0';   
  printf("Given strings:\n%s\n%s\n", a, b);
  printf("The length of the LCS is %d.\n", lcslen);
  printf("A possibly non-unique LCS is %s.\n", lcs);
  free(lcs); free2Dmem(table, la+1);
  return 0;
}

