/* file: fkp.c
   author: David De Potter
   description: fracktional knapsack problem (FKP)
                using a greedy algorithm
   time complexity: O(nlogn), where n is the number of items
*/ 

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int index;          // index of the item
  int weight;         // weight of the item
  double unitValue;   // unit value of the item
} Item;

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

Item *createUnits(int *values, int *weights, int n) {
  /* creates an array of items with their unit values */
  Item *unitValues = safeCalloc(n, sizeof(Item));
  for (int i = 0; i < n; i++) {
    unitValues[i].index = i;
    unitValues[i].weight = weights[i];
    unitValues[i].unitValue = (double)values[i] / weights[i];
  }
  return unitValues;
}

void printItems(double *maxSet, int n) {
  /* prints the selected items */
  for (int i = 0; i < n; i++) {
    if (maxSet[i] > 0){
      if (maxSet[i] == 1)
        printf("Item %d selected in full\n", i+1);
      else
        printf("Item %d selected for %.2f of its weight\n", i+1, maxSet[i]);
    }
  }
}

int compare(const void *a, const void *b) {
  /* compares two items by their unit values */
  Item *item1 = (Item *)a;
  Item *item2 = (Item *)b;
  if (item1->unitValue < item2->unitValue) return 1;
  if (item1->unitValue > item2->unitValue) return -1;
  return 0;
}

double selectItems(Item *unitValues, int n, int W, double *maxSet) {
  /* selects the items that maximize the total value */
  double max = 0;
  for (int i = 0; i < n; i++) {
    double w = unitValues[i].weight;
    if (w <= W) {
      maxSet[unitValues[i].index] = 1;
      W -= w;
      max += w * unitValues[i].unitValue;
    } else {
      maxSet[unitValues[i].index] = (double)W / w;
      max += W * unitValues[i].unitValue;
      break;
    }
  }
  return max;
}

int main (int argc, char *argv[]) {
  int weights[] = {10,  25, 15, 20,  30,  18,  5,  12,  9,  13};
  int values[] =  {120, 90, 80, 200, 280, 180, 50, 20, 100, 250};
  int n = 10;   // number of items
  int W = 60;   // capacity of the knapsack
  Item *unitValues = createUnits(values, weights, n);
  qsort(unitValues, n, sizeof(Item), compare);
  double *maxSet = safeCalloc(n, sizeof(double));
  double max = selectItems(unitValues, n, W, maxSet);
  printf("Maximum value: %.2f euros\n", max);
  printf("Selected items: \n");
  printItems(maxSet, n);
  free(unitValues);
  free(maxSet);
  return 0;
}