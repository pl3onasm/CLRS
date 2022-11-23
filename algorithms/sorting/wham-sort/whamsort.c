/* file: whamsort.c
* author: David De Potter
* description: WHAM sort
*/

#include <stdio.h>
#include <stdlib.h>
#define MIN(a,b) ((a) < (b) ? (a) : (b)); 

void *safeMalloc (int n) {
  /* checks if memory has been allocated successfully */
  void *p = malloc(n);
  if (p == NULL) {
    printf("Error: malloc(%d) failed. Out of memory?\n", n);
    exit(EXIT_FAILURE);
  }
  return p;
}

void printArray (int *arr, int n) {
  // prints an array of size n
  printf("[");
  for (int i = 0; i < n; i++) {
    printf("%d", arr[i]);
    if (i < n-1) printf(", ");
  }
  printf("]\n");
}

void merge (int left, int invIndex, int right, int arr[]) {
  int idx = 0, l = left, r = invIndex;
  int *temp = safeMalloc((right-left)*sizeof(int));
  while (l < invIndex && r < right) {
    if (arr[l] < arr[r]) temp[idx++] = arr[l++];
    else temp[idx++] = arr[r++];
  }
  while (l < invIndex) temp[idx++] = arr[l++];
  while (r < right) temp[idx++] = arr[r++];
  for (int i = left; i < right; i++) arr[i] = temp[i-left];
  free(temp);
}

int getInvIndex (int left, int right, int *arr) {
  // returns the index of the first inversion in arr[left..right]
  int idx = left + 1;
  while (idx < right && arr[idx - 1] <= arr[idx]) idx++;
  return idx;
}

void whamSort (int left, int right, int arr[]) {
  while (left < right) {
    int invIndex = getInvIndex (left, right, arr); 
    if (invIndex >= right) break;
    int newRight = MIN(2*invIndex - left, right);
    whamSort (invIndex, newRight, arr);
    merge (left, invIndex, right, arr);
  }
}

int main(int argc, char *argv[]) {
  int intExample[] = {5, 6, 7, 8, -9, 10, 2, 3, 4, 1, 2,
                      13, 7, -11, 20, 1, 15, -7, 16, 0};
  printf("Unsorted\n");
  printArray(intExample, 20);
  whamSort (0, 20, intExample);
  printf("Sorted\n");
  printArray (intExample, 20);
  return 0;
}