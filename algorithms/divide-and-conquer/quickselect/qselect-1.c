/* file: qselect-1.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: 
     implementation of quickselect, using a random pivot to
     partition the array while searching for the ith smallest
     element in the array.
   time complexity: 
     expected O(n), worst case O(n^2)
*/

#include <stdio.h>
#include <stdlib.h>

void *safeCalloc (int n) {
  /* checks if memory allocation was successful */
  void *p = calloc(n, sizeof(int));
  if (p == NULL) {
    printf("Error: malloc(%d) failed. Out of memory?\n", n);
    exit(EXIT_FAILURE);}
  return p;
}

int *readArray (int n) {
  /* reads input and stores it in an array */
  int *arr;
  arr = safeCalloc(n);
  for (int i = 0; i < n; ++i) scanf("%d", &arr[i]);
  return arr;
}

void swap (int i, int j, int *arr) {
  /* swaps arr[i] and arr[j] */
  int tmp = arr[i];
  arr[i] = arr[j];
  arr[j] = tmp;
}

int partition(int *arr, int left, int right) {
  /* partitions arr[left..right] around a random pivot */
  int idx = left + rand() % (right - left + 1);
  int pivot = arr[idx];
  swap(idx, right, arr);   // move pivot to the end
  idx = left;              // start of the partition's high end
  for (int i = left; i < right; i++)
    // move all elements ≤ pivot to the left
    if (arr[i] <= pivot)
      swap(i, idx++, arr);
  swap(idx, right, arr);   // move pivot to its final place
  return idx;
}

int quickSelect(int *arr, int left, int right, int i) {
  /* returns the ith smallest element in arr[left..right], i.e. the
     element that would be at index i if the array were sorted */
  if (left == right) 
    return arr[left];
  int q = partition(arr, left, right);
  int k = q - left + 1; // k = number of elements ≤ pivot
  if (i == k) 
    return arr[q];
  else if (i < k)   // search in the low end of the array
    return quickSelect(arr, left, q-1, i);
  else              // search in the high end of the array
    return quickSelect(arr, q+1, right, i-k);
      // we update i to be relative to the new subarray,
      // i.e. we search for the i-kth element in the high end
}

int main () {
  int n, i;
  scanf("%d %d", &n, &i);
  int *arr = readArray(n);
  printf("%d\n", quickSelect(arr, 0, n-1, i));
  free(arr);
  return 0;
}