/* 
  file: lis-4.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: longest increasing subsequence
    solution in linearithmic time
*/ 

#include "../../../lib/clib/clib.h"

//===================================================================
// Populates the memoization table and returns the length of the LIS
size_t computeLIS (int *arr, size_t len, size_t *table, 
                   size_t *parents) {
  
  size_t subLen = 1;
  for (size_t i = 1; i < len; i++) {
    if (arr[i] > arr[table[subLen - 1]]) {
      parents[i] = table[subLen - 1]; 
      table[subLen++] = i;
    } else {
      size_t low = 0, high = subLen - 1;
      while (low < high) {
        size_t mid = low + (high - low) / 2;
        if (arr[table[mid]] < arr[i]) 
          low = mid + 1;
        else high = mid;
      }
      table[low] = i;
      if (low > 0) 
        parents[i] = table[low - 1];
    }
  }
  return subLen;
}

//===================================================================
// Reconstructs a LIS from the parent array and prints it
void reconstructLIS (int *arr, size_t *parents, size_t p, 
                     size_t subLen) {
  
  CREATE_ARRAY(int, lis, subLen);
  
  for (int i = subLen - 1; i >= 0; i--) {
    lis[i] = arr[p]; 
    p = parents[p];
  }

  PRINT_ARRAY(lis, "%d", subLen);
  free(lis);
}

//===================================================================

int main () {

  READ(int, arr, "%d", len);

  CREATE_ARRAY(size_t, table, len);
  CREATE_ARRAY(size_t, prev, len);

  size_t subLen = computeLIS(arr, len, table, prev);

  printf("Max length: %zu\nSubsequence:\n ", subLen);
  reconstructLIS(arr, prev, table[subLen - 1], subLen);
  
  free(prev); free(table); free(arr);
  return 0;
}