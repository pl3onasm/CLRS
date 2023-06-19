/* file: mmp-2.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: Strassen's matrix multiplication algorithm
                generalized to matrices of any size
   time complexity: O(n^2.8074)
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

void strassen(int **A, int **B, int **C, int sz) {
  /* computes C = A x B using Strassen's algorithm */
  if (sz == 1) C[0][0] = A[0][0] * B[0][0];  // base case
  else {
    int n = sz/2, **S[10], **P[7], **Sub[4]; 
    // allocate memory for submatrices
    for (int i = 0; i < 10; ++i) S[i] = newM(n);
    for (int i = 0; i < 7; ++i) P[i] = newM(n);
    for (int i = 0; i < 4; ++i) Sub[i] = newM(n);

    // compute S₁ to S₁₀ and submatrices A₁₁, A₂₂, B₁₁, B₂₂
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j){
        S[0][i][j] = B[i][j+n] - B[i+n][j+n];   // S₁ = B₁₂ - B₂₂
        S[1][i][j] = A[i][j] + A[i][j+n];       // S₂ = A₁₁ + A₁₂
        S[2][i][j] = A[i+n][j] + A[i+n][j+n];   // S₃ = A₂₁ + A₂₂
        S[3][i][j] = B[i+n][j] - B[i][j];       // S₄ = B₂₁ - B₁₁
        S[4][i][j] = A[i][j] + A[i+n][j+n];     // S₅ = A₁₁ + A₂₂
        S[5][i][j] = B[i][j] + B[i+n][j+n];     // S₆ = B₁₁ + B₂₂
        S[6][i][j] = A[i][j+n] - A[i+n][j+n];   // S₇ = A₁₂ - A₂₂
        S[7][i][j] = B[i+n][j] + B[i+n][j+n];   // S₈ = B₂₁ + B₂₂
        S[8][i][j] = A[i][j] - A[i+n][j];       // S₉ = A₁₁ - A₂₁
        S[9][i][j] = B[i][j] + B[i][j+n];       // S₁₀ = B₁₁ + B₁₂
        Sub[0][i][j] = A[i][j];                 // A₁₁ 	      
        Sub[1][i][j] = B[i+n][j+n];             // B₂₂
        Sub[2][i][j] = A[i+n][j+n];             // A₂₂
        Sub[3][i][j] = B[i][j];                 // B₁₁
      }
    
    // recursively compute P₁ to P₇
    strassen(Sub[0], S[0], P[0], n);            // P₁ = A₁₁ x S₁
    strassen(S[1], Sub[1], P[1], n);            // P₂ = S₂ x B₂₂
    strassen(S[2], Sub[3], P[2], n);            // P₃ = S₃ x B₁₁
    strassen(Sub[2], S[3], P[3], n);            // P₄ = A₂₂ x S₄
    strassen(S[4], S[5], P[4], n);              // P₅ = S₅ x S₆
    strassen(S[6], S[7], P[5], n);              // P₆ = S₇ x S₈
    strassen(S[8], S[9], P[6], n);              // P₇ = S₉ x S₁₀
    
    // update the result matrix C
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j){
        C[i][j] += P[4][i][j] + P[3][i][j]      // C₁₁ = P₅ + P₄ - P₂ + P₆
                 - P[1][i][j] + P[5][i][j];  
        C[i][j+n] += P[0][i][j] + P[1][i][j];   // C₁₂ = P₁ + P₂
        C[i+n][j] += P[2][i][j] + P[3][i][j];   // C₂₁ = P₃ + P₄
        C[i+n][j+n] += P[4][i][j] + P[0][i][j]  // C₂₂ = P₅ + P₁ - P₃ - P₇
                     - P[2][i][j] - P[6][i][j];
      }
    
    // free auxiliary matrices
    for (int i = 0; i < 10; ++i) freeM(S[i], n);
    for (int i = 0; i < 7; ++i) freeM(P[i], n);
    for (int i = 0; i < 4; ++i) freeM(Sub[i], n);
  }
}

int pow2(int m, int l) {
  /* computes the next power of 2 ≥ m and l */
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
  
  readM(A, m, n); readM(B, k, l);

  // compute C = A x B
  strassen(A, B, C, p);
  printM(C, m, l);

  freeM(A, p); freeM(B, p);
  freeM(C, p);
  return 0;
}