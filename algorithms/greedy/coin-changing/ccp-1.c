/* file: ccp-1.c
   author: David De Potter
   description: coin changing problem (CCP)
                using a greedy approach 
   time complexity: O(n), where n is the number of coins in the coin system
*/ 

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int coins[] = {25, 10, 5, 1};
  char *singular[] = {"quarter", "dime", "nickel", "penny"};
  char *plural[] = {"quarters", "dimes", "nickels", "pennies"};
  int amounts[] = {81, 67, 19, 28};
  int n = sizeof(coins)/sizeof(int);

  for (int i = 0; i < n; ++i) {
    printf("Amount of %d cents = ", amounts[i]);
    int j = 0;
    int amount = amounts[i];
    int coin = coins[j];
    while (amount > 0 && j < n) {
      if (amount < coin) coin = coins[++j];
      else {
        int q = amount / coin;
        printf("%d %s", q, q > 1 ? plural[j] : singular[j]); 
        amount %= coin;
        if (amount > 0) printf(" + ");
      }
    }
    printf("\n"); 
  }
  return 0;
}