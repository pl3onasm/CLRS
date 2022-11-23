/* file: selectionsort.c
* author: David De Potter
* description: selection sort
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
    printf("%d", (arr)[i]);
    if (i < n-1) printf(", ");
  }
  printf("]\n");
}

void swap(int *arr, int i, int j) {
  // swaps elements at indices i and j in array arr
  int temp = arr[i];
  arr[i] = arr[j];
  arr[j] = temp;
}

void selectionSort(int *arr, int n) {
  // sorts an array of size n using selection sort
  for (int i = 0; i < n-1; i++) {
    int min = i;
    for (int j = i+1; j < n; j++) {
      if (arr[j] < arr[min]) min = j;
    }
    swap(arr, i, min);
  }
}

int main (int argc, char *argv[]) {
  int intExample[] = {10, 8, -9, 6, 7, 5, 2, 3, 4, -1, 2,
                      13, 7, 11, 20, 1, -15, 7, 16, 0};
  printf("Unsorted:\n");
  printArray(intExample, 20);
  selectionSort(intExample, 20);
  printf("Sorted:\n");
  printArray(intExample, 20);
  return 0;
}
