/* file: asp-3.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: activity selection problem (ASP)
                using an iterative top-down greedy algorithm
   assumptions: the activities are sorted in increasing order of finish time
   time complexity: Θ(n)
*/ 

#include <stdio.h>
#include <stdlib.h>

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
  for (int i = 0; i < n; i++) {
    if (maxSet[i] > 0)
      printf("Activity %d: [%d, %d)\n", i+1, acts[i].start, acts[i].finish);
  }
}

int selectActs(act *acts, int n, int *maxSet) {
  int k = 0, max = 1;
  maxSet[k] = 1;                  // select a₁
  for (int i = 1; i < n; i++) {
    if (acts[i].start >= acts[k].finish) {  // if aᵢ starts after aₖ finishes
      maxSet[i] = 1;              // select aᵢ
      k = i;                      // continue from aᵢ
      max++;
    }
  }
  return max;
}

int main (int argc, char *argv[]) {
  act acts[] = {{1, 4}, {3, 5}, {0, 6}, {5, 7}, {3, 9},
    {5, 9}, {6, 10}, {7, 11}, {8, 12}, {2, 14}, {12, 16}};
  int n = 11;   // number of activities
  int *maxSet = safeCalloc(n, sizeof(int));
  int max = selectActs(acts, n, maxSet);
  printf("Maximum number of activities: %d\n", max);
  printf("Selected activities:\n");
  printActs(acts, maxSet, n);
  free(maxSet);
  return 0; 
}