/* file: ccp-2.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: coin changing problem (CCP)
    using a bottom-up dynamic programming approach
   time complexity: O(nk), where n is the number of coins 
    in the coin system and k is the amount to be changed
*/ 

#include <stdio.h>
#include <stdlib.h>

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

int maxArray (int *a, int n) {
  /* returns the maximum value in array a of size n */
  int max = a[0];
  for (int i = 1; i < n; ++i) {
    if (a[i] > max) max = a[i];
  }
  return max;
}

int *change(int amount, int *coins, int cs) {
  /* returns an array where the i-th element contains the denomination 
     of a coin used as a last step in an optimal solution to the 
     (sub)problem of making change for i cents */
  int *minCoins = safeCalloc(amount + 1, sizeof(int));
  int *denom = safeCalloc(amount + 1, sizeof(int));
  for (int i = 1; i <= amount; ++i) {     // bottom-up approach: increasing amounts
    minCoins[i] = amount;
    for (int j = 0; j < cs; ++j) {        // try all coins
      if (i >= coins[j] && minCoins[i - coins[j]] + 1 < minCoins[i]) {
        minCoins[i] = minCoins[i - coins[j]] + 1;  // update minimum
        denom[i] = coins[j];                       // update denomination 
      }
    }
  }
  free(minCoins);
  return denom; 
}

void printChange(int *denom, int amount) {
  /* prints the optimal solution to the problem of 
     making change for a given amount */
  int a = amount;
  printf("Amount of %d cents = ", amount);
  while (a > 0) {
    printf("%d", denom[a]);
    a -= denom[a];
    if (a > 0) printf(" + ");
  }
  printf("\n");
}

int main(int argc, char *argv[]) {
  int coins[] = {25, 20, 10, 5, 1};     // non-canonical coin system
  int cs = sizeof(coins)/sizeof(int);
  int amounts[] = {40, 67, 19, 28, 45, 81}; // amounts to be changed
  int as = sizeof(amounts)/sizeof(int);
  
  int max = maxArray(amounts, as);      // get maximum amount to be changed
  int *denom = change(max, coins, cs);  // compute optimal solution for max

  for (int i = 0; i < as; ++i) 
    printChange(denom, amounts[i]);     
    
  free(denom);
  return 0;
}