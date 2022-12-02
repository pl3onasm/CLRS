/* file: lcs-3.c
* author: David De Potter
* description: longest common subsequence
* bottom-up dynamic programming implementation
* Notice that we don't need to reverse the lcs string,
* because we are building it from the bottom up, instead
* of from the top down, as we did in lcs-1.c.
* Also, we can now work with sequences that are much
* longer than we could with the naive recursive 
* implementation thanks to the O(mn) running time.
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

void fillTable (int **table, const char *a, const char *b, int la, int lb){
  /* fills the table with the lengths of the longest common subsequences */
  for (int i=1; i<=la; ++i)     // fill table in row-major order
    for (int j=1; j<=lb; ++j){
      if (a[i-1] == b[j-1]) table[i][j] = table[i-1][j-1] + 1; 
      else table[i][j] = MAX(table[i-1][j],table[i][j-1]); 
    }
} 

void reconstructLcs (int **table, char *a, char *b, int x, int y, char *str, int z){
  /* fills str with the longest common subsequence */
  if (x == 0 || y == 0){
    str[z] = '\0';
    return;
  }
  if (a[x-1] == b[y-1]) {
    str[z] = a[x-1]; 
    reconstructLcs(table, a, b, x-1, y-1, str, z-1);
  } else if (table[x][y-1] > table[x-1][y])  
    reconstructLcs(table, a, b, x, y-1, str, z); 
  else
    reconstructLcs(table, a, b, x-1, y, str, z); 
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
  fillTable(table, a, b, la, lb);
  int lcslen = table[la][lb]; 
  char *lcs = safeCalloc(lcslen+1, sizeof(char));
  reconstructLcs(table, a, b, la, lb, lcs, lcslen-1);
  printf("Given strings:\n%s\n%s\n", a, b);
  printf("The length of an LCS is %d.\n", lcslen);
  printf("An optimal LCS is %s.\n", lcs);
  free(lcs); free2Dmem(table, la+1);
  return 0;
}