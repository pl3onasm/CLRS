/* file: polyinversioncount.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: 
     We want to find the number of inversions in an array, which
     may contain any data type. For this we use the polymorphic
     version of mergesort. 
   note: 
     The idea of the polymorhic version is to use a function 
     pointer to a comparison function, which is then passed to
     the inversion count function so as to be able to compare 
     elements.
     This way, we can use the same inversion count function for 
     any data type, as long as there is a comparison function 
     for that data type. 
   time complexity: O(nlogn), since it's 
     essentially just mergesort with a counter.
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

int merge(void *arr, int left, int mid, int right, int width, 
int (*comp)(const void*, const void*)) {
  void *temp = safeMalloc((right - left + 1) * width);
  int l = left, r = mid + 1, t = 0, count = 0;
  while (l <= mid && r <= right) {
    if (comp((char*)arr + l*width, (char*)arr + r*width) <= 0) {
      // no inversions in this case
      memcpy((char*)temp + (t++)*width, (char*)arr + (l++)*width, width);
    } else {
      // total number of inversions to add is the number of
      // elements currently left in the left half
      memcpy((char*)temp + (t++)*width, (char*)arr + (r++)*width, width);
      count += mid - l + 1;
    }  
  }
  memcpy((char*)temp + t*width, (char*)arr + l*width, (mid - l + 1)*width);
  memcpy((char*)temp + t*width, (char*)arr + r*width, (right - r + 1)*width);
  memcpy((char*)arr + left*width, temp, (right - left + 1)*width);
  free(temp);
  return count;
}

int inversionCount(void *arr, int left, int right, int width, 
int (*comp)(const void*, const void*)) { 
  /* using polymorphic merge sort with a counter keeping the
     number of inversions */
  int count = 0;
  if (left < right) {
    int mid = left + (right - left)/2;
    count += inversionCount(arr, left, mid, width, comp);
    count += inversionCount(arr, mid + 1, right, width, comp);
    count += merge(arr, left, mid, right, width, comp);
  }
  return count;
}

int main (int argc, char *argv[]){
  int intExample[] = {10, 8, -9, 6, 7, 5, 2, 3, 4, -1, 2,
                      13, 7, 11, 20, 1, -15, 7, 16, 0};
  char *strExample[] = {"Harry", "Stephen", "Samuel", "James", 
                        "Chris", "Robert", "Michael", "William", 
                        "David", "Richard", "Charles", "Joseph", 
                        "Thomas", "John", "Daniel", "Matthew", 
                        "Anthony", "Jonathan", "Mark", "Paul"};
  printf("Example 1:\n");
  printArray(intExample, 20, 'i');
  printf("Number of inversions: ");
  printf("%d\n", inversionCount(intExample, 0, 19, sizeof(int), compInt));
  printf("\nExample 2:\n");
  printArray(strExample, 20, 's');
  printf("Number of inversions: ");
  printf("%d\n", inversionCount(strExample, 0, 19, sizeof(char*), compStr));
  return 0;
}