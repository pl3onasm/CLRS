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

//:::::::::::::::::::::::::::: input ::::::::::::::::::::::::::::::://

double readInput(item **items, double *W, int *n) {
  /* reads the input from stdin and stores it in the heap */
  double weight, value, totalWeight = 0; int i = 0;
  *items = safeCalloc(100, sizeof(item));
  scanf("%lf", W);
  while (scanf("%lf %lf", &weight, &value) == 2) {
    // if the heap is full, add more space
    (*items)[i].index = i + 1;
    (*items)[i].weight = weight;
    (*items)[i].value = value;
    (*items)[i].unitValue = value / weight;
    totalWeight += weight;
    i++;
    if (i % 100 == 0) 
      *items = safeRealloc(*items, (i+100)*sizeof(item));
  }
  *n = i;
  return totalWeight;
}

//::::::::::::::::::::::::::: selection :::::::::::::::::::::::::::://

void selectItems(item *items, double W) {
  /* selects the items to be put in the knapsack and returns the total value */
  double totalValue = 0; int i = 0;
  printf("Knapsack capacity: %.2lf kg\n", W);
  printf("\nItems selected in full:\n");
  while (W > 0) {
    if (items[i].weight > W) {  // item does not fully fit in knapsack
      printf("\nItem %d selected for %.2f%%: %.2f kg, %.2lf euros\n", 
        items[i].index, W/items[i].weight*100, W, items[i].unitValue*W);
      totalValue += items[i].unitValue*W;
      break;
    }
    // keep adding items to the knapsack
    printf("Item %d: %.2lf kg, %.2lf euros\n", 
      items[i].index, items[i].weight, items[i].value);
    W -= items[i].weight;
    totalValue += items[i].value;
    i++;
  }
  printf("Total value: %.2f euros\n", totalValue);
}

int compareItems(const void *a, const void *b) {
  /* compares two items by decreasing unit value */
  item *itemA = (item *) a;
  item *itemB = (item *) b;
  if (itemA->unitValue < itemB->unitValue) return 1;
  if (itemA->unitValue > itemB->unitValue) return -1;
  return 0;
}

//:::::::::::::::::::::::::::::: main :::::::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  double W;     // knapsack capacity
  int n;        // number of items
  item *items;  // array of items
  double w = readInput(&items, &W, &n);
  if (w <= W) printf("All items selected\n");
  else {
    // sort the items by decreasing unit value
    qsort(items, n, sizeof(item), compareItems);
    selectItems(items, W);
  }
  free(items);
  return 0;
}