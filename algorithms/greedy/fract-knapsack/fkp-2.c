/* file: fkp-2.c
   author: David De Potter
   description: fracktional knapsack problem (FKP)
                using a greedy algorithm and a max heap
   time complexity: O(n), where n is the number of items
*/ 

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int index;          // index of the item
  int weight;         // weight of the item
  double unitValue;   // unit value of the item
} Item;

typedef struct Heap {int size; Item *items;} Heap;

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

Heap *newHeap(int size) {
  /* creates a new heap of size size */
  Heap *hp = safeCalloc(sizeof(Heap), 1); hp->size = size;
  hp->items = safeCalloc(sizeof(Item), size);
  return hp ;
}

void swap (Heap *hp, int a, int b) {
  /* swaps the elements at indices a and b in the heap */
  Item temp = hp->items[a]; 
  hp->items[a] = hp->items[b]; 
  hp->items[b] = temp;
}

void maxHeapify(Heap *hp, int i){
  /* restores the max heap property for the heap */
  int max = i, left = 1<<i, right = (1<<i) + 1;
  if (left < hp->size && hp->items[left].unitValue > hp->items[max].unitValue)
    max = left;
  if (right < hp->size && hp->items[right].unitValue > hp->items[max].unitValue)
    max = right;
  if (max != i) {
    swap(hp, i, max); 
    maxHeapify(hp, max);
  }
}

void initMaxHeap(int *values, int *weights, int n, Heap *hp) {
  /* initializes the max heap */
  for (int i = hp->size >>1; i >= 0; --i) maxHeapify(hp, i);
}

void freeHeap(Heap *hp) {
  /* frees the heap */
  free(hp->items);
  free(hp);
}

Item extractMax(Heap *hp){
  /* extracts the max element from the heap and 
  restores the max heap property */
  Item max = hp->items[0];
  hp->items[0] = hp->items[hp->size-1];
  hp->size--; 
  maxHeapify(hp, 0);
  return max; 
}

void readHeap(Heap *hp, int *weights, int *values, int n) {
  /* reads the heap from the input */
  for (int i = 0; i < n; i++) {
    hp->items[i].index = i;
    hp->items[i].weight = weights[i];
    hp->items[i].unitValue = (double)values[i] / weights[i];
  }
}

void printItems(double *maxSet, double max, int n) {
  /* prints the selected items */
  printf("Total value: %.2f\n", max);
  printf("Selected items:\n"); 
  for (int i = 0; i < n; i++) {
    if (maxSet[i] > 0){
      if (maxSet[i] == 1)
        printf("Item %d in full\n", i+1);
      else
        printf("Item %d for %.2f of its weight\n", i+1, maxSet[i]);
    }
  }
}

double selectItems(Heap *hp, double *maxSet, int W, int n) {
  /* selects the items to be put in the knapsack */
  double totalValue = 0;
  while (W > 0 && hp->size > 0) {
    Item item = extractMax(hp);
    if (item.weight <= W) {
      maxSet[item.index] = 1;
      W -= item.weight;
      totalValue += item.weight * item.unitValue;
    } else {
      maxSet[item.index] = (double)W / item.weight;
      totalValue += W * item.unitValue;
      W = 0;
    }
  }
  return totalValue;
}

int main (int argc, char *argv[]) {
  int weights[] = {11,  25, 14,  5,  33,  15,  16, 12,  9,  13};
  int values[] =  {120, 94, 85, 200, 271, 183, 50, 20, 100, 250};
  int n = 10;   // number of items
  int W = 60;   // capacity of the knapsack
  Heap *hp = newHeap(n);
  readHeap(hp, weights, values, n);
  initMaxHeap(weights, values, n, hp);
  double *maxSet = safeCalloc(sizeof(double), n);
  double max = selectItems(hp, maxSet, W, n);
  printItems(maxSet, max, n);
  freeHeap(hp);
  free(maxSet);
  return 0;
}