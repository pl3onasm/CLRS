/* file: asp-3.c
   author: David De Potter
   description: activity selection problem (ASP)
                using an iterative top-down greedy algorithm
   assumptions: the activities are sorted in increasing order of finish time
   time complexity: O(n)
*/ 

#include <stdio.h>
#include <stdlib.h>

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

void printActs(int *start, int *finish, int *maxSet, int n) {
  /* prints the selected activities in [i..j] */
  for (int i = 0; i < n; i++) {
    if (maxSet[i] > 0)
      printf("Activity %d: [%d, %d)\n", i+1, start[i], finish[i]);
  }
}

int selectActs(int *start, int *finish, int n, int *maxSet) {
  int k = 0, max = 1;
  maxSet[k] = 1;                  // select a₁
  for (int i = 2; i < n; i++) {
    if (finish[k] <= start[i]) {  // aᵢ starts after aₖ finishes
      maxSet[i] = 1;              // select aᵢ
      k = i;
      max++;
    }
  }
  return max;
}

int main (int argc, char *argv[]) {
  int start[]  = {1, 3, 0, 5, 3, 5, 6,  7,  8,  2,  12};
  int finish[] = {4, 5, 6, 7, 9, 9, 10, 11, 12, 14, 16};
  int n = 11;   // number of activities
  int *maxSet = safeCalloc(n, sizeof(int));
  int max = selectActs(start, finish, n, maxSet);
  printf("Maximum number of activities: %d\n", max);
  printf("Selected activities:\n");
  printActs(start, finish, maxSet, n);
  free(maxSet);
  return 0; 
}