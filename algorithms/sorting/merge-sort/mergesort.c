/* file: mergesort.c
* author: David De Potter
* description: merge sort, using a temporary array
*/

#include <stdlib.h>
#include <stdio.h>

void *safeMalloc (int n) {
  /* allocates n bytes of memory and checks whether the allocation
     was successful */
  void *ptr = malloc(n);
  if (ptr == NULL) {
    printf("Error: malloc(%d) failed. Out of memory?\n", n);
    exit(EXIT_FAILURE);
  }
  return ptr;
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

void merge(int *arr, int left, int mid, int right) {
  int *temp = safeMalloc((right - left + 1)*sizeof(int));
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

int main (int argc, char *argv[]){
  int example[] = {10, -8, 9, 6, 7, -5, 2, 3, 4, -1, 2,
                   13, -7, 11, 20, 1, -15, 7, 16, 0};
  printf("Unsorted:\n");
  printArray(example, 20);
  mergeSort(example, 0, 19);
  printf("Sorted:\n");
  printArray(example, 20);
  return 0;
}