/* file: mmp-1.c
   author: David De Potter
   description: standard matrix multiplication for input
    matrices of arbitrary size, using a divide-and-conquer approach
   time complexity: O(n³), where n is the padding size
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

int **newM(int n) {
  /* allocates a n x n matrix */
  int **m = safeCalloc(n, sizeof(int *));
  for (int i = 0; i < n; ++i)
    m[i] = safeCalloc(n, sizeof(int));
  return m;
}

void freeM(int **m, int n) {
  /* frees a n x n matrix */
  for (int i = 0; i < n; ++i) free(m[i]);
  free(m);
}

void printM(int **arr, int m, int n) {
  /* prints a m x n matrix */
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j){
      printf("%d", arr[i][j]);
      if (j < n - 1) printf(" ");
    }
    printf("\n");
  }
}

void readM(int **arr, int m, int n) {
  /* reads a m x n matrix */
  for (int i = 0; i < m; ++i)
    for (int j = 0; j < n; ++j)
      scanf("%d", &arr[i][j]);
}

void multiplyM(int **A, int **B, int **C, int n, int i, int k, int j) {
  /* recursively computes the product of n x n matrices A and B
     and stores the result in n x n matrix C */

  // base case
  if (n == 1) {
    C[i][j] += A[i][k] * B[k][j];
    return;
  }
  // partition A, B and C into 4 submatrices using indices
  // and recursively multiply the submatrices
  int n2 = n / 2;
  int i2 = i + n2;
  int j2 = j + n2;
  int k2 = k + n2;

  // C₁₁ = A₁₁ * B₁₁ + A₁₂ * B₂₁
  multiplyM(A, B, C, n2, i, k, j);
  multiplyM(A, B, C, n2, i, k2, j);
  // C₁₂ = A₁₁ * B₁₂ + A₁₂ * B₂₂
  multiplyM(A, B, C, n2, i, k, j2);
  multiplyM(A, B, C, n2, i, k2, j2);
  // C₂₁ = A₂₁ * B₁₁ + A₂₂ * B₂₁
  multiplyM(A, B, C, n2, i2, k, j);
  multiplyM(A, B, C, n2, i2, k2, j);
  // C₂₂ = A₂₁ * B₁₂ + A₂₂ * B₂₂
  multiplyM(A, B, C, n2, i2, k, j2);
  multiplyM(A, B, C, n2, i2, k2, j2);
}

int pow2(int m, int l) {
  /* computes the next power of 2 ≥ n and l */
  if (m < l) m = l;
  int p = 1;
  while (p < m) p <<= 1;
  return p;
}

int main(int argc, char *argv[]) {
  int m, n, k, l;   // matrix dimensions

  scanf("%d %d", &m, &n);
  scanf("%d %d", &k, &l);

  if (n != k) { 
    printf("Incompatible matrices.\n");
    return 1;
  }

  int p = pow2(m, l); // padding size

  // allocate pxp matrices
  int **A = newM(p);
  int **B = newM(p);
  int **C = newM(p);

  // read matrices
  readM(A, m, n);
  readM(B, k, l);

  multiplyM(A, B, C, p, 0, 0, 0);
  printM(C, m, l);

  freeM(A, p);
  freeM(B, p);
  freeM(C, p);
  return 0; 
}
  
