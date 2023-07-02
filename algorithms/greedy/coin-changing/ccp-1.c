/* file: ccp-1.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: coin changing problem (CCP)
                using a greedy approach 
   time complexity: O(n), where n is the number of coins in the coin system
*/ 

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int coins[] = {25, 10, 5, 1};         // coins in descending order
  char *singular[] = {"quarter", "dime", "nickel", "penny"};
  char *plural[] = {"quarters", "dimes", "nickels", "pennies"};
  int amounts[] = {81, 67, 19, 28, 45}; // amounts to be changed
  int n = sizeof(coins)/sizeof(int);    // number of coins in the coin system
  int m = sizeof(amounts)/sizeof(int);  // number of amounts to be changed

  for (int i = 0; i < m; ++i) {
    printf("Amount of %d cents = ", amounts[i]);
    int j = 0;
    int amount = amounts[i];
    int coin = coins[j];
    while (amount > 0 && j < n) {
      if (amount < coin) coin = coins[++j];  // move to next coin
      else {
        int q = amount / coin;    // number of coins we can use of this type
        printf("%d %s", q, q > 1 ? plural[j] : singular[j]); 
        amount %= coin;           // remaining amount to be changed
        if (amount > 0) printf(" + ");
      }
    }
    printf("\n"); 
  }
  return 0;
}