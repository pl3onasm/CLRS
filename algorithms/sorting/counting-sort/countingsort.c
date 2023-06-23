/* file: countingsort.c
* author: David De Potter
* description: counting sort
* This version can only be used to sort arrays 
* containing non-negative integers.
*/

#include <stdlib.h>
#include <stdio.h>

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

void printArray (int arr[], int n) {
  // prints an array of size n
  printf("[");
  for (int i = 0; i < n; i++) {
    printf("%d", (arr)[i]);
    if (i < n-1) printf(", ");
  }
  printf("]\n");
}

int *createIntArray (int size) {
  // creates an array of size size
  int *arr = safeCalloc(size, sizeof(int));
  return arr;
}

void countingSort(int arr[], int n) {
  int max = arr[0], *count, *sorted;
  for (int i = 1; i < n; i++) {
    if (arr[i] > max) max = arr[i];
  }
  count = createIntArray(max+1);
  for (int i = 0; i < n; i++) count[arr[i]]++;
  for (int i = 1; i <= max; i++) count[i] += count[i-1];
  sorted = createIntArray(n);
  for (int i = n-1; i >= 0; i--) {
    sorted[count[arr[i]]-1] = arr[i];
    count[arr[i]]--;
  }
  for (int i = 0; i < n; i++) arr[i] = sorted[i];
  free(count);
  free(sorted);
}

int main (int argc, char *argv[]){
  int example[] = {10, 8, 9, 6, 7, 5, 2, 3, 4, 1, 2,
                   13, 7, 11, 20, 1, 15, 7, 16, 18};
  printf("Unsorted:\n");
  printArray(example, 20);
  countingSort(example, 20);
  printf("Sorted:\n");
  printArray(example, 20);
  return 0;
}