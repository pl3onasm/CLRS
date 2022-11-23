/* file: polymergesort.c
* author: David De Potter
* description: polymorphic merge sort, can be used to sort any 
* type of data, including structs
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

void printArray (void *arr, int n, char type) {
  // prints an array of size n
  printf("[");
  for (int i = 0; i < n; i++) {
    if (type == 'i') printf("%d", ((int *)arr)[i]);
    else printf("%s", ((char **)arr)[i]);
    if (i < n-1) printf(", ");
  }
  printf("]\n");
}

int compInt (const void *a, const void *b) {
  /* compares two integers */
  return (*(int*)a - *(int*)b);
}

int compStr (const void *a, const void *b) {
  /* compares two strings */
  return strcmp(*(char**)a, *(char**)b);
}

void merge(void *arr, int left, int mid, int right, int width, 
int (*comp)(const void*, const void*)) {
  void *temp = safeMalloc((right - left + 1) * width);
  int i = left, j = mid + 1, t = 0;   
  while (i <= mid && j <= right) {
    if (comp((char*)arr + i*width, (char*)arr + j*width) < 0) 
      memcpy((char*)temp + (t++)*width, (char*)arr + (i++)*width, width);
    else memcpy((char*)temp + (t++)*width, (char*)arr + (j++)*width, width);
  }
  memcpy((char*)temp + t*width, (char*)arr + i*width, (mid - i + 1)*width);
  memcpy((char*)temp + t*width, (char*)arr + j*width, (right - j + 1)*width);
  memcpy((char*)arr + left*width, temp, (right - left + 1)*width);
  free(temp);
}

void mergeSort(void *arr, int left, int right, int width, 
int (*comp)(const void*, const void*)) { 
  if (left < right) {
    int mid = left + (right - left)/2;
    mergeSort(arr, left, mid, width, comp);
    mergeSort(arr, mid + 1, right, width, comp);
    merge(arr, left, mid, right, width, comp);
  }
}

int main (int argc, char *argv[]){
  int intExample[] = {10, 8, -9, 6, 7, 5, 2, 3, 4, -1, 2,
                      13, 7, 11, 20, 1, -15, 7, 16, 0};
  char *strExample[] = {"Harry", "Stephen", "Samuel", "James", 
                        "Chris", "Robert", "Michael", "William", 
                        "David", "Richard", "Charles", "Joseph", 
                        "Thomas", "John", "Daniel", "Matthew", 
                        "Anthony", "Jonathan", "Mark", "Paul"};
  printf("Unsorted:\n");
  printArray(intExample, 20, 'i');
  mergeSort(intExample, 0, 19, sizeof(int), compInt);
  printf("Sorted:\n");
  printArray(intExample, 20, 'i');
  printf("Unsorted:\n");
  printArray(strExample, 20, 's');
  mergeSort(strExample, 0, 19, sizeof(char*), compStr);
  printf("Sorted:\n");
  printArray(strExample, 20, 's');
  return 0;
}