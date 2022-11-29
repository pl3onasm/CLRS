/* file: lcs.c
* author: David De Potter
* description: longest common subsequence
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

void fillTable (int ***table, const char *a, const char *b, int la, int lb){
  *table = safeCalloc(la+1, sizeof(int*)); 
  for (int i=0; i<=la; ++i) (*table)[i] = safeCalloc(lb+1, sizeof(int)); 
  for (int i=1; i<=la; ++i)
    for (int j=1; j<=lb; ++j){
      if (a[i-1] == b[j-1]) (*table)[i][j] = (*table)[i-1][j-1] + 1; 
      else (*table)[i][j] = MAX((*table)[i][j-1], (*table)[i-1][j]); 
    }
} 

void getLcs (int **table, char *a, char *b, int x, int y, char **str, int z){
  if (x == 0 || y == 0) return;
  if (a[x-1] == b[y-1]) {
    (*str)[z] = a[x-1]; 
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
  int **table, la = strlen(a), lb = strlen(b); 
  fillTable(&table, a, b, la, lb);
  int clslen = table[la][lb]; 
  char *str = safeCalloc(clslen+1, sizeof(char));
  getLcs(table, a, b, la, lb, &str, clslen-1); 
  str[clslen]='\0'; free2Dmem(table, la); 
  return str;
}