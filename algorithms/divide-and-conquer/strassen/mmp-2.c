/* file: mmp-2.c
   author: David De Potter
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

int **addSubm(int **A, int n, int r1, int c1, int r2, int c2, char op) {
  /* adds or subtracts the n x n submatrices of A marked by indices r1,  
     c1, r2 and c2, and returns the result in a new n x n matrix */
  int **S = newM(n);
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      if (op == '+') S[i][j] = A[r1 + i][c1 + j] + A[r2 + i][c2 + j];
      else S[i][j] = A[r1 + i][c1 + j] - A[r2 + i][c2 + j];
  return S;
}

int **getSubm (int **A, int n, int r, int c) {
  /* returns the n x n submatrix of A marked by indices r and c */
  int **M = newM(n);
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      M[i][j] = A[r + i][c + j];
  return M;
}

void update (int **C, int **P1, int **P2, int **P3, int **P4, 
  int **P5, int **P6, int **P7, int n) {
  /* computes C₁₁ += P₅ + P₄ - P₂ + P₆, C₁₂ += P₁ + P₂
     C₂₁ += P₃ + P₄, C₂₂ += P₅ + P₁ - P₃ - P₇ */
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j){
      C[i][j] += P5[i][j] + P4[i][j] - P2[i][j] + P6[i][j];
      C[i][j+n] += P1[i][j] + P2[i][j];
      C[i+n][j] += P3[i][j] + P4[i][j];
      C[i+n][j+n] += P5[i][j] + P1[i][j] - P3[i][j] - P7[i][j];
    }
}

void strassen(int **A, int **B, int **C, int n) {
  /* computes the product of the n x n matrices A and B, and stores the
     result in C, using Strassen's matrix multiplication algorithm */
   
  // base case
  if (n == 1) {
    C[0][0] = A[0][0] * B[0][0]; 
    return;
  }

  // recursive case
  int s = n / 2;

  // allocate memory for products
  int **P1 = newM(s); int **P2 = newM(s);
  int **P3 = newM(s); int **P4 = newM(s);
  int **P5 = newM(s); int **P6 = newM(s);
  int **P7 = newM(s);
  
  // compute P₁ = A₁₁ * (B₁₂ - B₂₂)
  int **S1 = addSubm(B, s, 0, s, s, s, '-');
  int **A11 = getSubm(A, s, 0, 0);
  strassen(A11, S1, P1, s);

  // compute P₂ = (A₁₁ + A₁₂) * B₂₂
  int **S2 = addSubm(A, s, 0, 0, 0, s, '+');
  int **B22 = getSubm(B, s, s, s);
  strassen(S2, B22, P2, s);

  // compute P₃ = (A₂₁ + A₂₂) * B₁₁
  int **S3 = addSubm(A, s, s, 0, s, s, '+');
  int **B11 = getSubm(B, s, 0, 0);
  strassen(S3, B11, P3, s);

  // compute P₄ = A₂₂ * (B₂₁ - B₁₁)
  int **S4 = addSubm(B, s, s, 0, 0, 0, '-');
  int **A22 = getSubm(A, s, s, s);
  strassen(A22, S4, P4, s);

  // compute P₅ = (A₁₁ + A₂₂) * (B₁₁ + B₂₂)
  int **S5 = addSubm(A, s, 0, 0, s, s, '+');
  int **S6 = addSubm(B, s, 0, 0, s, s, '+');
  strassen(S5, S6, P5, s);

  // compute P₆ = (A₁₂ - A₂₂) * (B₂₁ + B₂₂)
  int **S7 = addSubm(A, s, 0, s, s, s, '-');
  int **S8 = addSubm(B, s, s, 0, s, s, '+');
  strassen(S7, S8, P6, s);

  // compute P₇ = (A₁₁ - A₂₁) * (B₁₁ + B₁₂)
  int **S9 = addSubm(A, s, 0, 0, s, 0, '-');
  int **S10 = addSubm(B, s, 0, 0, 0, s, '+');
  strassen(S9, S10, P7, s);

  // update the result matrix
  update(C, P1, P2, P3, P4, P5, P6, P7, s);

  // free temporary matrices
  int **ms[] = {P1, P2, P3, P4, P5, P6, P7, S1, S2, S3, S4, 
                S5, S6, S7, S8, S9, S10, A11, A22, B11, B22};
  for (int i = 0; i < 21; ++i) freeM(ms[i], s);
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

  readM(A, m, n); readM(B, k, l);

  strassen(A, B, C, p);
  printM(C, m, l);

  freeM(A, p); freeM(B, p);
  freeM(C, p);
  return 0;
}