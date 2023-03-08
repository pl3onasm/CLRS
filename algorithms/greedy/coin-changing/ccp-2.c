/* file: ccp-2.c
   author: David De Potter
   description: coin changing problem (CCP)
                using a bottom-up dynamic programming approach
   time complexity: O(nk), where n is the number of coins in the coin system
                    and k is the amount to be changed
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

int *change(int amount, int *coins, int cs) {
  /* returns an array where the i-th element contains the denomination 
     of a coin used in an optimal solution to the problem of making 
     change for i cents */
  int *minCoins = safeCalloc(amount + 1, sizeof(int));
  int *denom = safeCalloc(amount + 1, sizeof(int));
  for (int i = 1; i <= amount; ++i) {
    minCoins[i] = amount;
    for (int j = 0; j < cs; ++j) {
      if (i >= coins[j] && minCoins[i - coins[j]] + 1 < minCoins[i]) {
        minCoins[i] = minCoins[i - coins[j]] + 1;
        denom[i] = coins[j];
      }
    }
  }
  free(minCoins);
  return denom; 
}

void printChange(int *denom, int amount) {
  /* prints the optimal solution to the problem of 
     making change for amount cents */
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
  int coins[] = {25, 20, 10, 5, 1}; // non-canonical coin system
  int cs = sizeof(coins)/sizeof(int);
  int amounts[] = {40, 67, 19, 28};
  int as = sizeof(amounts)/sizeof(int);

  for (int i = 0; i < as; ++i) {
    int *denom = change(amounts[i], coins, cs);
    printChange(denom, amounts[i]); 
    free(denom);
  }
  return 0;
}