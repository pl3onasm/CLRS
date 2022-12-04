/* file: lcs-1.c
* author: David De Potter
* description: longest common subsequence
* naive recursive implementation
* As the example strings get longer, the running time 
* of this algorithm grows exponentially.
* Note that we need to reverse the LCS string at the end,
* because we filled it in reverse order: we can't get around
* this, because we are working our way from the end of the
* strings to the beginning and don't know the length of the
* LCS in advance. In the second version, we'll
* see a more natural implementation by working from the
* beginning of the strings to the end.
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

char *reverse (char *s) {
  /* reverses string s in place */
  int i, j; char c;
  for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
  return s;
}

void computeLcs (char *a, char *b, int la, int lb, char *lcs, 
  int lcslen, int *maxlen, char *maxlcs) {
  if (la == 0 || lb == 0) {
    if (lcslen > *maxlen) {
      *maxlen = lcslen;
      strcpy(maxlcs, lcs);
      lcs[lcslen] = '\0';
    }
    return; 
  }
  if (a[la-1] == b[lb-1]) { // last characters are equal
    lcs[lcslen] = a[la-1];
    computeLcs(a, b, la-1, lb-1, lcs, lcslen+1, maxlen, maxlcs);
  } else {                  // last characters are different
    // try without last character of a
    computeLcs(a, b, la-1, lb, lcs, lcslen, maxlen, maxlcs);
    // try without last character of b
    computeLcs(a, b, la, lb-1, lcs, lcslen, maxlen, maxlcs);
  }
}

int main (int argc, char *argv[]) {
  char *a = "ACCGATATCAGGATCGGAC", 
       *b = "GTAGTAATTACGAACA";
  int la = strlen(a), lb = strlen(b);
  char *lcs = safeCalloc(la < lb ? lb+1 : la+1, sizeof(char));
  char *maxlcs = safeCalloc(la < lb ? lb+1 : la+1, sizeof(char));
  int maxlen = 0;
  
  computeLcs(a, b, la, lb, lcs, 0, &maxlen, maxlcs);
  printf("Given strings:\n%s\n%s\n", a, b);
  printf("The length of an LCS is %d.\n", maxlen);
  printf("An optimal LCS is %s.\n", reverse(maxlcs));
  free(lcs); free(maxlcs);
  return 0;
}