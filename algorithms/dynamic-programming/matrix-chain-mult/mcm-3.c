/* file: mcm-3.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description:
    Matrix chain multiplication using dynamic programming, 
    Bottom-up approach
    We use an example where we want to compute the minimal
    cost of the matrix chain multiplication A₁ * ... * A₆.
    The dimensions of the matrices are: A₁ = 30x35, 
    A₂ = 35x15, A₃ = 15x5, A₄ = 5x10, A₅ = 10x20, A₆ = 20x25
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void computeMinCosts (int dims[], int minCosts[][7], int splitPoints[][7], int n) {
  /* computes the minimum costs and optimal split points for all subproblems */
  for (int i = 0; i < n; i++) {     // initializes the main diagonal
    minCosts[i][i] = 0;             // costs of matrices of size 1 are 0
  }
  for (int l = 2; l <= n; l++) {    // fills table in order of increasing chain length l
    for (int i = 1; i < n - l + 1; i++) {   // chain starts at i
      int j = i + l - 1;                    // chain ends at j    
      minCosts[i][j] = INT_MAX;             // initialize minCosts[i][j] to infinity
      for (int k = i; k < j; k++) { // try all possible split points k
        int q = minCosts[i][k] + minCosts[k + 1][j] + 
                dims[i-1] * dims[k] * dims[j];
        if (q < minCosts[i][j]) {   // smaller cost found ?
          minCosts[i][j] = q;
          splitPoints[i][j] = k;
        }
      }
    }
  }
}

void printOptimalParens (int splitPoints[][7], int i, int j) {
  /* Prints the optimal parenthesization of the matrix chain
     starting at i and ending at j. */
  if (i == j) {
    printf("A%d", i);
  } else {
    printf("(");
    printOptimalParens(splitPoints, i, splitPoints[i][j]);
    printf(" x ");
    printOptimalParens(splitPoints, splitPoints[i][j] + 1, j);
    printf(")");
  }
}

int main (int argc, char *argv[]) {
  int dims[] = {30,35,15,5,10,20,25};
    // holds the dimensions of the matrices A₁, ..., A₆ 
    // as follows: A₁ = dims[0] x dims[1], 
    // A₂ = dims[1] x dims[2], ..., A₆ = dims[5] x dims[6]
  int minCosts[7][7], splitPoints[7][7];

  computeMinCosts(dims, minCosts, splitPoints, 7);

  printf("The minimal cost of the matrix chain product "
         "is %d scalar multiplications.\n", minCosts[1][6]);
  printf("The optimal parenthesization is: ");
  printOptimalParens(splitPoints, 1, 6);
  printf("\n");

  return 0;
}