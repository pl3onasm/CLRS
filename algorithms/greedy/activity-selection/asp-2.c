/* file: asp-2.c
   author: David De Potter
   description: activity selection problem (ASP)
                using a recursive top-down greedy algorithm
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
      printf("Activity %d: [%d, %d)\n", i, start[i], finish[i]);
  }
}

void selectActs(int *start, int *finish, int i, int n, int *max, int *maxSet) {
  int k = i+1; 
  while (k < n && start[k] < finish[i]) k++; 
    // find first activity that starts after aᵢ finishes
  if (k < n) {
    ++*max; 
    ++maxSet[k];
    selectActs(start, finish, k, n, max, maxSet);
  }
}

int main (int argc, char *argv[]) {
  int start[]  = {0, 1, 3, 0, 5, 3, 5, 6,  7,  8,  2,  12};
  int finish[] = {0, 4, 5, 6, 7, 9, 9, 10, 11, 12, 14, 16};
  int n = 11+1;   // number of activities + 1 sentinel activity
  int *maxSet = safeCalloc(n, sizeof(int));
  int max = 0;
  selectActs(start, finish, 0, n, &max, maxSet);
  printf("Maximum number of activities: %d\n", max);
  printf("Selected activities:\n");
  printActs(start, finish, maxSet, n);
  free(maxSet);
  return 0; 
}