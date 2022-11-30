/* file: cut-rod4.c
* author: David De Potter
* description: extended version to also print 
* the optimal solution.
* The time complexity is in O(n²).
*/ 

#include <stdio.h>
#include <stdlib.h>

int cut_rod (int prices[], int n, int revenues[], int solutions[]) {
  if (revenues[n] >= 0)  // return the stored value 
    return revenues[n];
  for (int j = 1; j <= n; j++) {
    int q = -1;
    for (int i = 1; i <= j; i++) {  // i is the length of the first cut
      if (q < prices[i] + revenues[j-i]) {
        q = prices[i] + revenues[j-i];
        solutions[j] = i;  // best cut so far for rod of length j
      }
    }
    revenues[j] = q;  // store the computed value
  }
  return revenues[n];
}

void print_solution (int n, int solutions[], int revenues[], int prices[]) {
  if (solutions[n] < 0) {
    cut_rod (prices, n, revenues, solutions);
  }
  while (n > 0) {
    printf("%d", solutions[n]);
    if (n - solutions[n] > 0)
      printf(", ");
    n = n - solutions[n];    // subtract the length of the cut
  }
  printf("\n");
}

int main (int argc, char *argv[]) {
  int prices[] = {0, 1, 5, 8, 9, 10, 17, 17, 
                  20, 24, 30, 32, 35, 39, 43, 
                  43, 45, 49, 50, 54, 57, 60, 
                  65, 68, 70, 74, 80, 81, 84, 
                  85, 87, 91, 95, 99, 101, 104, 
                  107, 112, 115, 116, 119}; 
  int revenues[40], solutions[40];
  for (int i = 0; i < 40; i++) {
    revenues[i] = -1;
    solutions[i] = -1;
  }
  revenues[0] = 0;
  
  printf("The maximum revenue is %d for a rod of length"
        " %d.\nThe optimal cut(s): ", 
        cut_rod(prices, 7, revenues, solutions), 7); 
  print_solution (7, solutions, revenues, prices);

  printf("\nThe optimal cuts for a rod of length %d are: ", 37);
  print_solution (37, solutions, revenues, prices);
  return 0;
}
