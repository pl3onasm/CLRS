/* file: mcm-1.c
  author: David De Potter
  description:
  Matrix chain multiplication using naive recursion, in which
  the same subproblems are solved over and over again.
  We use an example where we want to compute the minimal
  cost of the matrix chain multiplication A₁ * ... * A₂₀.
  The dimensions of the matrices are: A₁ = 30x35, 
  A₂ = 35x15, A₃ = 15x5, A₄ = 5x10, ... , A₂₀ = 40x50.
  Note that it already takes a long time to compute the
  minimal cost for this example.
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MIN(a,b) ((a) < (b) ? (a) : (b));

int computeMinCosts (int dims[], int i, int j) {
  /* computes the minimum cost of the matrix chain multiplication
     starting at i and ending at j */
  if (i == j) {
    return 0;   // cost of multiplying a single matrix is 0
  } else {
    int minCost = INT_MAX;
    for (int k = i; k < j; k++) { // try all possible split points k
      int q = computeMinCosts(dims, i, k) + computeMinCosts(dims, k + 1, j) + 
              dims[i-1] * dims[k] * dims[j];
      minCost = MIN(minCost, q);
    }
    return minCost;
  }
}
    
int main (int argc, char *argv[]) {
  int dims[] = {30,35,15,5,10,20,25,8,10,15,20,50,
                30,20,50,80,90,10,20,30,40,50};
    // holds the dimensions of the matrices A₁, ..., A₂₀ 
    // as follows: A₁ = dims[0] x dims[1], 
    // A₂ = dims[1] x dims[2], ..., A₂₀ = dims[19] x dims[20]

  printf("The minimal cost of the matrix chain product is %d "
         "scalar multiplications.\n", computeMinCosts(dims, 1, 21));

  return 0;
}
