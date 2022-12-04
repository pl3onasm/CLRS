/* file: lcs-3.c
* author: David De Potter
* description: longest common subsequence
* Top-down approach with memoization
* Time complexity: O(nm)
* Space complexity: O(nm)
* You could also choose to work as in lcs-2.c, i.e. working from the
* start of the strings. This won't make the code easier in this case 
* as we still need to work with indices to be able to fill the 
* memoization table, and we still need to reconstruct the lcs afterwards
* from the table, so we might as well work from the end of the strings.
*
* Note that we can now work with sequences that are much longer than
* in the naive recursive implementation, thanks to the O(mn) running time.
*/ 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX(a,b) ((a)>(b)?(a):(b))

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
  // initializes all entries to -1
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      table[i][j] = -1;
  return table;
}

int computeTable (int **table, char *a, char *b, int ia, int ib) {
  /* computes the table of longest common subsequences */
  if (table[ia][ib] < 0) {  // if entry is not yet computed
    if (ia == 0 || ib == 0) table[ia][ib] = 0;
    else if (a[ia-1] == b[ib-1]) 
      table[ia][ib] = computeTable(table, a, b, ia-1, ib-1) + 1;
    else 
      table[ia][ib] = MAX(computeTable(table, a, b, ia-1, ib), 
                          computeTable(table, a, b, ia, ib-1));
  }
  return table[ia][ib];
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
  int maxlen = computeTable(table, a, b, la, lb);
  char *lcs = safeCalloc(maxlen+1, sizeof(char));
  lcs[maxlen] = '\0';
  reconstructLcs(table, a, la, lb, lcs, maxlen-1);
  printf("Given strings:\n%s\n%s\n", a, b);
  printf("The length of the LCS is %d.\n", maxlen);
  printf("A possibly non-unique LCS is %s.\n", lcs);
  free(lcs); free2Dmem (table, la+1);
  return 0;
}