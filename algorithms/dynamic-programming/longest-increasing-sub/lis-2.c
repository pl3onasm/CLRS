/* file: lis-2.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: longest increasing subsequence
     bottom-up dynamic programming, using the LCS algorithm
*/ 

#include <stdlib.h>
#include <stdio.h>

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

int **createTable (int n, int m) {
  /* creates a table of size n x m */
  int **table = safeCalloc(n, sizeof(int *));
  for (int i = 0; i < n; i++) 
    table[i] = safeCalloc(m, sizeof(int));
  return table;
}

void merge(int *arr, int left, int mid, int right) {
  int *temp = safeCalloc((right - left + 1),sizeof(int));
  int l = left, r = mid + 1, t = 0;   
  while (l <= mid && r <= right) {
    if (arr[l] < arr[r]) temp[t++] = arr[l++];
    else temp[t++] = arr[r++];
  }
  while (l <= mid) temp[t++] = arr[l++];
  while (r <= right) temp[t++] = arr[r++];
  for (int i = left; i <= right; i++) arr[i] = temp[i - left];
  free(temp);
}

void mergeSort(int *arr, int left, int right) { 
  /* sorts the int array in ascending order */
  if (left < right) {
    int mid = left + (right - left)/2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
  }
}

int removeDuplicates(int *arr, int n) {
  /* removes duplicates from the array arr */
  int i = 0, j = 1;
  while (j < n) {
    if (arr[i] != arr[j]) arr[++i] = arr[j];
    j++;
  }
  return i + 1; // new length of the array
}

int *copyArray (int *arr, int n) {
  /* returns a copy of the array arr of size n */
  int *copy = safeCalloc(n, sizeof(int));
  for (int i = 0; i < n; i++) copy[i] = arr[i];
  return copy;
}

int fillTable (int **table, int *a, int *sorted, int la, int ls){
  /* fills the table with the lengths of the lcs's of the prefixes 
      of a and b and returns the length of the lcs of a and b */
  for (int i=1; i<=la; ++i)     // fill table in row-major order
    for (int j=1; j<=ls; ++j){
      if (a[i-1] == sorted[j-1]) table[i][j] = table[i-1][j-1] + 1; 
      else table[i][j] = MAX(table[i-1][j],table[i][j-1]); 
    }
  return table[la][ls];
} 

void reconstructLis (int **table, int *a, int x, int y, int *lis, int z){
  /* fills the string lis in reverse order (index z starts at the end) 
     with the longest increasing subsequence */
  if (x == 0 || y == 0) return;
  if (table[x][y] == table[x-1][y]) 
    reconstructLis(table, a, x-1, y, lis, z);
  else if (table[x][y] == table[x][y-1]) 
    reconstructLis(table, a, x, y-1, lis, z);
  else {
    lis[z] = a[x-1]; 
    reconstructLis(table, a, x-1, y-1, lis, z-1);
  }
} 

void free2Dmem (int **arr, int m) {
  for (int row = 0; row < m; ++row) free(arr[row]);
  free(arr);
}

void printArray (int *arr, int n) {
  // prints an array of size n
  printf("[");
  for (int i = 0; i < n; i++) {
    printf("%d", (arr)[i]);
    if (i < n-1) printf(", ");
  }
  printf("]\n");
}

int main (int argc, char *argv[]) {
  int arr[] = {12, 8, 2, 1, 4, 2, 9, 10, 18, 15, 12, 7, 3, 1, 
               20, 14, 30, 12, 24, 40, 5, 10, 5, 26, 11, 2};
  int n = 26;
  int *sorted = copyArray(arr, n);
  mergeSort(sorted, 0, n-1);
  int len = removeDuplicates(sorted, n);  // len = length of sorted
  int *lis = safeCalloc(len, sizeof(int));
  int **table = createTable(n+1, len+1);
  int maxLen = fillTable(table, arr, sorted, n, len);
  reconstructLis(table, arr, n, len, lis, maxLen-1);
  free2Dmem(table, n+1); free(sorted);

  printf("The given array is:\n");
  printArray(arr, n);
  printf("The length of the longest increasing subsequence is %d.\n", maxLen);
  printf("A possible LIS is:\n");
  printArray(lis, maxLen);
  free(lis);
  return 0;
}

