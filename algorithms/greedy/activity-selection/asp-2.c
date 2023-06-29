/* file: asp-2.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: activity selection problem (ASP)
                using a recursive top-down greedy algorithm
   assumptions: the activities are sorted in increasing order of finish time
   time complexity: Θ(n)
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int start, finish;
} act;

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

void printActs(act *acts, int *maxSet, int n) {
  /* prints the selected activities in [i..j] */
  for (int i = 0; i < n; i++) 
    if (maxSet[i] > 0)
      printf("Activity %d: [%d, %d)\n", i, acts[i].start, acts[i].finish);
}

void selectActs(act *acts, int i, int n, int *max, int *maxSet) {
  int k = i+1; 
  while (k < n && acts[k].start < acts[i].finish) ++k;
    // find index k of first activity that starts after aᵢ finishes
  if (k < n) {
    ++*max; 
    ++maxSet[k];
    selectActs(acts, k, n, max, maxSet);
  }
}

int main (int argc, char *argv[]) {
  act acts[] = {{0, 0}, {1, 4}, {3, 5}, {0, 6}, {5, 7}, 
    {3, 9}, {5, 9}, {6, 10}, {7, 11}, {8, 12}, {2, 14}, {12, 16}};
  int n = 11+1;   // number of activities + 1 sentinel activity
  int *maxSet = safeCalloc(n, sizeof(int));
  int max = 0;
  selectActs(acts, 0, n, &max, maxSet);
  printf("Maximum number of activities: %d\n", max);
  printf("Selected activities:\n");
  printActs(acts, maxSet, n);
  free(maxSet);
  return 0; 
}