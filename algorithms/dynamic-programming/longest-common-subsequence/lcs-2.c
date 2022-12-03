/* file: lcs-2.c
* author: David De Potter
* description: longest common subsequence
* This is a more natural implementation of the naive recursive 
* solution. 
* The subproblems no longer consist of decreasing the length of
* the sequences, but of increasing the starting position in the
* strings. This makes the code more natural: 
* we can work with increasing char pointers and stop when we reach
* the null character, and we don't need to reverse the LCS string
* at the end.
* Note that we still need to keep track of the length of the LCS
* in order to be able make a copy of the LCS string.
*/ 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

void computeLcs (char *a, char *b, char *lcs, 
  int lcslen, int *maxlen, char *maxlcs) {
  if (*a == '\0' || *b == '\0') {
    if (lcslen > *maxlen) {
      *maxlen = lcslen;
      strcpy(maxlcs, lcs);
      lcs[lcslen] = '\0';
    }
    return; 
  }
  if (*a == *b) { // first characters are equal
    lcs[lcslen] = *a;
    computeLcs(a+1, b+1, lcs, lcslen+1, maxlen, maxlcs);
  } else {        // first characters are different
    // try without first character of a
    computeLcs(a+1, b, lcs, lcslen, maxlen, maxlcs);
    // try without first character of b
    computeLcs(a, b+1, lcs, lcslen, maxlen, maxlcs);
  }
}

int main (int argc, char *argv[]) {
  char *a = "ACCGATATCAGGATCGGAC", 
       *b = "GTAGTAATTACGAACA";
  int la = strlen(a), lb = strlen(b);
  char *lcs = safeCalloc(la < lb ? lb+1 : la+1, sizeof(char));
  char *maxlcs = safeCalloc(la < lb ? lb+1 : la+1, sizeof(char));
  int maxlen = 0;
  
  computeLcs(a, b, lcs, 0, &maxlen, maxlcs);
  printf("Given strings:\n%s\n%s\n", a, b);
  printf("The length of an LCS is %d.\n", maxlen);
  printf("An optimal LCS is %s.\n", maxlcs);
  free(lcs); free(maxlcs);
  return 0;
}

