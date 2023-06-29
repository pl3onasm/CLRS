/* file: fkp-2.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: fractional knapsack problem (FKP)
      using a greedy approach and a divide-and-conquer algorithm
   time complexity: O(n), where n is the number of items
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

//:::::::::::::::::::::::::: quickselect ::::::::::::::::::::::::::://

void swap (int a, int b, item *arr) {
  /* swaps the elements at indices 
     a and b in the array arr */
  item temp = arr[a]; 
  arr[a] = arr[b]; 
  arr[b] = temp;
}

int partition(item *arr, int left, int right) {
  /* partitions the array arr around a random
     pivot and returns the index of the pivot */
  srand(time(NULL));  // seed the random number generator
  int idx = left + rand() % (right-left+1);
  item pivot = arr[idx];
  swap(idx, right, arr);
  idx = left;
  for (int i = left; i < right; i++){
    if (arr[i].unitValue > pivot.unitValue){  // descending order
      swap(i, idx, arr);
      idx++;
    }
  }
  swap(idx, right, arr);
  return idx;
}

item quickSelect(item *arr, int left, int right, int k) {
  /* returns the item with median unit value */
  int idx = partition(arr, left, right);
  if (idx == k) 
    return arr[idx];
  if (idx > k) 
    return quickSelect(arr, left, idx-1, k);
  return quickSelect(arr, idx+1, right, k);
}

//::::::::::::::::::: determine last item to add ::::::::::::::::::://

double sumWeights (item *items, int left, int right) {
  /* returns the sum of the weights of the items in [left, right] */
  double sum = 0;
  for (int i = left; i <= right; i++)
    sum += items[i].weight;
  return sum;
}

int getIndex (item *items, double W, int left, int right) {
  /* returns the index of the last item that contributes 
     fully or partially to knapsack of capacity W */
  
  // we use quickselect to find the median of the items array;
  // the array is then partitioned (not sorted) around the median
  int k = (right - left) / 2 + left;  // index of median if sorted
  if (k == left) return left;   
  item median = quickSelect(items, left, right, k);

  // W1 is the sum of the weights of the items 
  // with a unit value ≥ median unit value
  double W1 = sumWeights(items, left, k);

  if (W1 > W) // too heavy 
    return getIndex(items, W, left, k-1);
  // not heavy enough
  return getIndex(items, W-W1, k+1, right);
}

//:::::::::::::::::::::::: input / output :::::::::::::::::::::::::://

double readInput ( item **items, double *W, int *n) {
  /* reads the input from stdin and returns the total value of the items */
  double v, w, totalWeight = 0;
  int i = 0;
  scanf("%lf", W);
  *items = safeCalloc(100, sizeof(item));
  while (scanf("%lf %lf", &w, &v) == 2) {
    (*items)[i].index = i + 1;
    (*items)[i].value = v;
    (*items)[i].weight = w;
    (*items)[i].unitValue = v / w;
    totalWeight += w;
    i++;
    if (i % 100 == 0) 
      *items = safeRealloc(*items, (i+100) * sizeof(item));
  }
  *n = i;
  return totalWeight;
}

void printItems (item *items, int idx, double W, int n) {
  /* prints the selected items */
  double total = 0, totalWeight = 0;
  printf("Knapsack capacity: %.2lf kg\n", W);
  printf("\nItems selected in full:\n");
  for (int i = 0; i < idx; i++) {
    printf("Item %d: %.2f kg, %.2f euros\n", 
      items[i].index, items[i].weight, items[i].value);
    total += items[i].value;
    totalWeight += items[i].weight;
  }
  if (totalWeight < W) {
    double frac = (W - totalWeight) / items[idx].weight;
    printf("\nItem %d: selected for %.2f%% of its weight (= %.2f kg), %.2f euros\n",
      items[idx].index, frac * 100, frac * items[idx].weight,
      frac * items[idx].value);
    total += frac * items[idx].value;
  }
  printf("Total value: %.2f euros\n", total);
  return;
}

//:::::::::::::::::::::::::::: main :::::::::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  double W;   // knapsack capacity
  int n;      // number of items
  item *items;
  double w = readInput(&items, &W, &n);
  if (w <= W) printf("All items selected\n");
  else {
    int idx = getIndex(items, W, 0, n-1); 
    printItems(items, idx, W, n);
  }
  free(items);
  return 0;
}