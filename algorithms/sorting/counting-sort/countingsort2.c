/* file: countingsort2.c
* author: David De Potter
* description: counting sort
* This is an adaptation so that counting sort can also
* be used to sort arrays containing negative integers.
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

void printArray (int *arr, int n) {
  // prints an array of size n
  printf("[");
  for (int i = 0; i < n; i++) {
    printf("%d", arr[i]);
    if (i < n-1) printf(", ");
  }
  printf("]\n");
}

void countingSort(int length, int arr[]) {
  /* sorts an array of integers in O(n) time */
  int min, max, range, i, j, idx = 0;
  min = max = arr[0];
  for (i = 1; i < length; i++) {
    min = (arr[i] < min ? arr[i] : min);
    max = (arr[i] > max ? arr[i] : max);
  }
  range = max - min + 1;
  int *count = safeCalloc(range, sizeof(int));
  for (i = 0; i < length; i++) 
    count[arr[i] - min]++;
  for (i = 0; i < range; i++) 
    for (j = 0; j < count[i]; j++)
      arr[idx++] = min + i; 
  free(count);
}

int main (int argc, char *argv[]){
  int example[] = {10, -8, -9, 6, -7, 5, -2, 3, -4, 1, 2,
                   -13, 7, -11, 20, -1, 15, -7, -16, -20};
  printf("Unsorted:\n");
  printArray(example, 20);
  countingSort(20, example);
  printf("Sorted:\n");
  printArray(example, 20);
  return 0;
}