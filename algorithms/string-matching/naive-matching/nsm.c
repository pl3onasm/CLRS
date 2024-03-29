/* file: nsm.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: string matching, naive approach
*/

#include <stdio.h>
#include <stdlib.h>

typedef unsigned int uint;
typedef unsigned char uchar;

typedef enum {               
  false = 0,
  true = 1
} bool;

void *safeMalloc (int n) {
  /* allocates memory and checks whether the allocation was successful */
  void *ptr = malloc(n);
  if (ptr == NULL) {
    printf("Error: malloc(%d) failed. Out of memory?\n", n);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void *safeRealloc (void *ptr, int newSize) {
  /* reallocates memory and checks whether the allocation was successful */
  ptr = realloc(ptr, newSize);
  if (ptr == NULL) {
    printf("Error: realloc(%d) failed. Out of memory?\n", newSize);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

uchar *readString(uint *size, short type) {
  /* reads a string from stdin and stores its length in size */
  uchar c; uint len = 0; 
  uchar tok = type ? '\n' : EOF;   // type = 1: read until newline
  uchar *str = safeMalloc(100*sizeof(uchar));
  while (scanf("%c", &c) == 1 && c != tok) {
    if (c == '\n') c = ' '; // replace newline with space
    str[len++] = c; 
    if (len % 100 == 0) str = safeRealloc(str, (len+100) * sizeof(uchar));
  }
  str[len] = '\0';
  *size = len;
  return str;
}

void computeShifts (uchar *text, uint tLen, uchar *pattern, uint pLen) {
  /* determines all valid shifts of pattern in text and prints them */
  if (pLen > tLen) {
    printf("Pattern is longer than text. No shifts found.\n"); 
    return;
  }
  bool r = false; // result found?
  uint i, j, n = tLen - pLen;

  // loop over all possible shifts
  for (i = 0; i <= n; i++) {
    for (j = 0; j < pLen; j++) {
      if (text[i+j] != pattern[j]) break;
    }
    if (j == pLen) {  // valid shift
      if (!r) {
        r = true;
        printf("Shifts: %d", i);
      } else printf(", %d", i);
    }
  }
  if (r) printf("\n"); 
  else printf("No matches found.\n");
}

int main () {
  uint tLen, pLen;

  uchar *pattern = readString(&pLen, 1);
  uchar *text = readString(&tLen, 0);

  computeShifts(text, tLen, pattern, pLen);

  free(text);
  free(pattern);
  return 0;
}