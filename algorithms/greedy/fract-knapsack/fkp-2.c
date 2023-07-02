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

//:::::::::::::::::::::::::: partitioning :::::::::::::::::::::::::://

void swap (int a, int b, item *arr) {
  /* swaps the elements at indices 
     a and b in the array arr */
  item temp = arr[a]; 
  arr[a] = arr[b]; 
  arr[b] = temp;
}

int partition(item *arr, int left, int right, double *W1) {
  /* partitions the array arr around a random
     pivot and returns the index of the pivot */
  int idx = left + rand() % (right - left + 1);
  item pivot = arr[idx];
  swap(idx, right, arr);
  idx = left;
  *W1 = pivot.weight;  // include weight of pivot
  for (int i = left; i < right; i++){
    if (arr[i].unitValue >= pivot.unitValue){ 
      *W1 += arr[i].weight;
      swap(i, idx, arr);
      idx++;
    }
  }
  swap(idx, right, arr);
  return idx;
}

int getIndex (item *items, double W, int left, int right) {
  /* returns the index of an optimal boundary between the items  
     that still fit in the knapsack and the items that don't */
  double W1;
  if (left >= right) 
    return left;
  int idx = partition(items, left, right, &W1);
  if (W1 == W) 
    return idx;
  if (W1 > W)   // too heavy, look to the left of pivot
    return getIndex(items, W, left, idx-1);
  // too light, take all items on the left of pivot and look to the right
  return getIndex(items, W - W1, idx+1, right);
}

//:::::::::::::::::::::::: input / output :::::::::::::::::::::::::://

double readInput (item **items, double *W, int *n) {
  /* reads the input from stdin and returns the total weight of the items */
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

void printItems (item *items, int idx, double cap, int n) {
  /* prints the selected items */
  double total = 0;
  printf("Knapsack capacity: %.2lf kg\n", cap);
  printf("\nItems selected in full:\n");
  for (int i = 0; i < n && i <= idx+1; i++) {
    if (items[i].weight <= cap) {
      // item fits in knapsack, take it
      printf("Item %d: %.2f kg, %.2f euros\n", 
        items[i].index, items[i].weight, items[i].value);
      total += items[i].value;
      cap -= items[i].weight;
    } else if (cap) {  // item does not fit in knapsack, take a fraction
      printf("\nItem %d selected for %.2lf%%: %.2f kg, %.2f euros\n",
        items[i].index, 100 * cap / items[i].weight, cap, cap * items[i].unitValue);
      total += cap * items[i].unitValue;
      break;
    } else break; // knapsack is full
  }
  printf("\nTotal value: %.2f euros\n", total);
  return;
}

//:::::::::::::::::::::::::::: main :::::::::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  
  double cap;   // knapsack capacity
  int n;        // number of items
  item *items;
  double w = readInput(&items, &cap, &n);
  if (w <= cap) printf("All items selected\n");
  else {
    int idx = getIndex(items, cap, 0, n-1);
    printItems(items, idx, cap, n);
  }
  free(items);
  return 0;
}