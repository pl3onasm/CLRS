/* file: chain1.c
* author: David De Potter
* description:
* Matrix chain multiplication using dynamic programming
*/ 


#include <stdio.h>
#include <stdlib.h>

#define MAX(a,b) ((a)>(b)?(a):(b));

int mcm(int A[], int B[], int C[], int D[], int p[8]) {
  
}
    
int main (int argc, char *argv[]) {
  int A[5][4] = {{5,2,3,1},{4,6,8,8},{9,1,1,2},{7,1,5,6},{5,8,0,0}};
  int B[4][2] = {{4,2},{3,1},{5,8},{7,4}};
  int C[2][3] = {{0,2,1},{4,3,6}};
  int D[3][6] = {{9,2,4,4,3,6},{8,1,2,10,9,12},{3,5,1,0,18}};

  printf("The minimal cost of the matrix chain A*B*C*D is %d scalar multiplications.", mcm(A, 0, 4, B, 0, 3, C, 0, 2, D, 0, 5));

  return 0;
}
