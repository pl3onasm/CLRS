/* file: fkp-2.c
   author: David De Potter
   description: fractional knapsack problem (FKP)
      using a greedy approach and a divide-and-conquer algorithm
   time complexity: O(n), where n is the number of items
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct {
  int index;          // index of the item
  double weight;      // weight of the item
  double value;       // value of the item
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

Item *readItems (double *weights, double *values, int n) {
  /* reads the items from the arrays weights and values */
  Item *items = safeCalloc(sizeof(Item), n);
  for (int i = 0; i < n; ++i) {
    items[i].index = i;
    items[i].weight = weights[i];
    items[i].value = values[i];
    items[i].unitValue = values[i] / weights[i];
  }
  return items;
}

double sumWeights (Item *items, int left, int right) {
  /* returns the sum of the weights of the items 
     in interval [left, right) */
  double total = 0;
  for (int i = left; i < right; i++)
    total += items[i].weight;
  return total;
}

void printItems (Item *items, int lim, double W, int n) {
  /* prints the selected items */
  double totalW = sumWeights(items, 0, lim);
  double total = 0;
  printf("Selected items:\n");
  for (int i = 0; i < lim; i++) {
    printf("Item %d in full\n", items[i].index+1);
    total += items[i].value;
  }
  if (totalW < W && lim < n && totalW + items[lim].weight > W) {
    double w = W - totalW;
    double p = w / (items[lim].weight) * 100;
    total += w * items[lim].unitValue;
    printf("Item %d for %.2f%% of its weight (= %.2f kg)\n", 
            items[lim].index+1, p, w);
  } 
  printf("Total value: %.2f euros\n", total);
  return;
}

void swap (int a, int b, Item *arr) {
  /* swaps the elements at indices 
     a and b in the array arr */
  Item temp = arr[a]; 
  arr[a] = arr[b]; 
  arr[b] = temp;
}

int partition(Item *arr, int left, int right) {
  /* partitions the array arr around a random
     pivot and returns the index of the pivot */
  srand(time(NULL));  // seed the random number generator
  int idx = left + rand() % (right-left+1);
  Item pivot = arr[idx];
  swap(idx, right, arr);
  idx = left;
  for (int i = left; i < right; i++){
    if (arr[i].unitValue > pivot.unitValue){
      swap(i, idx, arr);
      idx++;
    }
  }
  swap(idx, right, arr);
  return idx;
}

Item quickSelect(Item *arr, int left, int right, int k) {
  /* returns the item with median unit value */
  int idx = partition(arr, left, right);
  if (idx == k) return arr[idx];
  if (idx > k) return quickSelect(arr, left, idx-1, k);
  return quickSelect(arr, idx+1, right, k);
}

int getLimit (Item *items, double W, int left, int right) {
  /* returns the index of the last item that contributes 
     fully or partially */
  
  // we use quickselect to find the median of the items array;
  // the array is then partitioned (not sorted) around the median
  int k = ceil((double)(left+right)/2);
  if (k == left) return left;
  Item med = quickSelect(items, left, right, k);
  
  // W1 is the sum of the weights of the items 
  // with a higher unit value than the median
  double W1 = sumWeights(items, left, k);
  int i = 0; 

  // W2 is the sum of the weights of the items
  // with the same unit value as the median
  double W2 = med.weight;
  for (i = k+1; i <= right; i++){
    if (items[i].unitValue == med.unitValue) W2 += items[i].weight;
    else break;
  }

  if (W1 <= W && W1+W2 >= W) {  // base case
    k--;  // index of the last item that contributed to W1
    while (W1 < W) {  // tries to add as many items as possible
      W1 += med.weight;
      k++; 
    }
    return k;
  }

  if (W1 > W) return getLimit(items, W, left, k-1);
  return getLimit(items, W-W1-W2, i, right);
}

int main (int argc, char *argv[]) {
  double weights[] = {11.1,  25.2, 14.5, 5.25, 33,  15.3,  16,  12.9, 9.7, 13.9};
  double values[]  = {125.5, 94.13, 85,  201, 27.6, 183, 50.75, 20.2, 105, 250};
  int n = 10;       // number of items
  double W = 61.5;  // capacity of the knapsack
  Item *items = readItems(weights, values, n);

  double W1 = sumWeights(items, 0, n);
  if (W1 <= W) {
    printf("All items selected.\n");
    return 0;
  }

  int lim = getLimit(items, W, 0, n-1); 
  printItems(items, lim, W, n);
  free(items);
  return 0;
}