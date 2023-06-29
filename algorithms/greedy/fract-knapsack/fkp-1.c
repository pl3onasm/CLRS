/* file: fkp-1.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: fractional knapsack problem (FKP)
      using a greedy algorithm and a max heap (priority queue)
   time complexity: O(nlogn), where n is the number of items
*/ 

#include <stdio.h>
#include <stdlib.h>

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

typedef struct {
  int index;          // index of the item as given in the input
  double weight;      // total weight of the item
  double value;       // total value of the item
  double unitValue;   // value per unit of weight of the item
} item;

typedef struct {
  int size;           // actual size of the heap
  int capacity;       // maximum size of the heap
  item *items;
} heap;

//::::::::::::::::::::::: memory management :::::::::::::::::::::::://

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

void *safeRealloc (void *ptr, int newSize) {
  /* reallocates memory and checks whether the allocation was successful */
  ptr = realloc(ptr, newSize);
  if (ptr == NULL) {
    printf("Error: realloc(%d) failed. Out of memory?\n", newSize);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

//:::::::::::::::::::::::: heap functions :::::::::::::::::::::::::://

heap *newHeap() {
  /* creates a new heap of size size */
  heap *hp = safeCalloc(1, sizeof(heap)); 
  hp->capacity = 100;
  hp->size = 0;
  hp->items = safeCalloc(hp->capacity, sizeof(item));
  return hp ;
}

void freeHeap(heap *hp) {
  /* frees the heap */
  free(hp->items);
  free(hp);
}

void swap (heap *hp, int a, int b) {
  /* swaps the elements at indices a and b in the heap */
  item temp = hp->items[a]; 
  hp->items[a] = hp->items[b]; 
  hp->items[b] = temp;
}

void maxHeapify(heap *hp, int i){
  /* restores the max heap property for the heap */
  int max = i, left = 2*i+1, right = 2*i+2;
  if (left < hp->size && hp->items[left].unitValue > hp->items[max].unitValue)
    max = left;
  if (right < hp->size && hp->items[right].unitValue > hp->items[max].unitValue)
    max = right;
  if (max != i) {
    swap(hp, i, max); 
    maxHeapify(hp, max);
  }
}

void initMaxHeap(heap *hp) {
  /* initializes the max heap */
  for (int i = hp->size/2; i >= 0; i--)
    maxHeapify(hp, i);
}

item extractMax(heap *hp){
  /* extracts the max element from the heap and 
  restores the max heap property */
  item max = hp->items[0];
  hp->items[0] = hp->items[hp->size-1];
  hp->size--; 
  maxHeapify(hp, 0);
  return max; 
}

//::::::::::::::::::::::::: other functions :::::::::::::::::::::::://

double readInput(heap *hp, double *W) {
  /* reads the input from stdin and stores it in the heap */
  double weight, value, totalWeight = 0; int index = 1; 
  scanf("%lf", W);
  while (scanf("%lf %lf", &weight, &value) == 2) {
    // if the heap is full, add more space
    if (hp->size == hp->capacity) {
      hp->capacity *= 2;
      hp->items = safeRealloc(hp->items, hp->size * sizeof(item));
    }
    // store the item in the heap
    hp->items[hp->size].index = index;
    hp->items[hp->size].weight = weight;
    hp->items[hp->size].value = value;
    hp->items[hp->size].unitValue = value / weight;
    hp->size++; index++;
    totalWeight += weight;
  }
  return totalWeight;
}

void selectItems(heap *hp, double W) {
  /* selects the items to be put in the knapsack and returns the total value */
  double totalValue = 0;
  printf("Knapsack capacity: %.2lf kg\n", W);
  printf("\nItems selected in full:\n");
  while (W > 0 && hp->size > 0) {
    item item = extractMax(hp);
    if (item.weight <= W) {
      printf("Item %d: %.2lf kg, %.2lf euros\n", item.index, item.weight, item.value);
      W -= item.weight;
      totalValue += item.value;
    } else {
      printf("\nItem %d: selected for %.2f%% of its weight (= %.2f kg), %.2lf euros\n", 
        item.index, W / item.weight * 100, W, item.unitValue * W);
      totalValue += item.unitValue * W;
      break;
    }
  }
  printf("Total value: %.2f euros\n", totalValue);
}

//:::::::::::::::::::::::::::::: main :::::::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  double W;   // knapsack capacity
  heap *hp = newHeap();
  double w = readInput(hp, &W);
  if (w <= W) printf("All items selected\n");
  else {
    initMaxHeap(hp);
    selectItems(hp, W);
  }
  freeHeap(hp);
  return 0;
}