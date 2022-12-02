/* file: lcs-3.c
* author: David De Potter
* description: longest common subsequence
* bottom-up dynamic programming implementation
*/ 

#include <string.h>
#include <malloc.h>
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

void getLcs (int **table, char *a, char *b, int x, int y, char *str, int z){
  /* fills str with the longest common subsequence */
  if (x == 0 || y == 0) return;
  if (a[x-1] == b[y-1]) {
    str[z] = a[x-1]; 
    return getLcs(table, a, b, x-1, y-1, str, z-1);  
  }
  if (table[x][y-1] > table[x-1][y])  
    return getLcs(table, a, b, x, y-1, str, z); 
  return getLcs(table, a, b, x-1, y, str, z); 
} 

void free2Dmem (int** arr, int m) {
  for (int row = 0; row < m; ++row) free(arr[row]);
  free(arr);
}

char *lcs(char *a, char *b){
  int la = strlen(a), lb = strlen(b); 
  int **table = createTable(la+1, lb+1); 
  
  fillTable(table, a, b, la, lb);
  int clslen = table[la][lb]; 
  char *str = safeCalloc(clslen+1, sizeof(char));
  getLcs(table, a, b, la, lb, &str, clslen-1); 
  str[clslen]='\0'; free2Dmem(table, la); 
  return str;
}