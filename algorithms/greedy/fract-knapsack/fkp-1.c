/* file: fkp-1.c
   author: David De Potter
   description: fractional knapsack problem (FKP)
      using a greedy algorithm and a max heap (priority queue)
   time complexity: O(nlogn), where n is the number of items
*/ 

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int index;          // index of the item
  double weight;      // weight of the item
  double value;       // value of the item
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

void initMaxHeap(Heap *hp) {
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

void readHeap(Heap *hp, double *weights, double *values) {
  /* reads the heap from the input */
  for (int i = 0; i < hp->size; i++) {
    hp->items[i].index = i;
    hp->items[i].weight = weights[i];
    hp->items[i].value = values[i];
    hp->items[i].unitValue = values[i] / weights[i];
  }
}

void printItems(double *maxSet, double max, int n, double rem) {
  /* prints the selected items */
  printf("Selected items:\n"); 
  for (int i = 0; i < n; i++) {
    if (maxSet[i] > 0){
      if (maxSet[i] == 1)
        printf("Item %d in full\n", i+1);
      else
        printf("Item %d for %.2f%% of its weight (= %.2f kg)\n", i+1, maxSet[i], rem);
    }
  }
  printf("Total value: %.2f euros\n", max);
}

double selectItems(Heap *hp, double *maxSet, double W, double *rem) {
  /* selects the items to be put in the knapsack and returns the total value */
  double totalValue = 0;
  while (W > 0 && hp->size > 0) {
    Item item = extractMax(hp);
    if (item.weight <= W) {
      maxSet[item.index] = 1;
      W -= item.weight;
      totalValue += item.value;
    } else {
      maxSet[item.index] = W / item.weight * 100;
      totalValue += item.unitValue * W;
      *rem = W; W = 0;
    }
  }
  return totalValue;
}

double sum (double *arr, int n) {
  /* returns the sum of the first n elements of arr */
  double sum = 0;
  for (int i = 0; i < n; i++) sum += arr[i];
  return sum;
}

int main (int argc, char *argv[]) {
  double weights[] = {11.1,  25.2, 14.5, 5.25, 33,  15.3,  16,  12.9, 9.7, 13.9};
  double values[]  = {125.5, 94.13, 85,  201, 27.6, 183, 50.75, 20.2, 105, 250};
  int n = 10;       // number of items
  double W = 61.5;  // capacity of the knapsack
  if (sum(weights, n) <= W) {
    printf("All items selected\n");
    return 0;
  }
  Heap *hp = newHeap(n);
  readHeap(hp, weights, values);
  initMaxHeap(hp);
  double *maxSet = safeCalloc(sizeof(double), n);
  double rem = 0;
  double max = selectItems(hp, maxSet, W, &rem);
  printItems(maxSet, max, n, rem);
  freeHeap(hp);
  free(maxSet);
  return 0;
}