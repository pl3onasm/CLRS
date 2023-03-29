/* file: lps-1.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: Longest Palindromic Subsequence
     This is a naive recursive implementation. 
   If you only want to compute the length of the LPS,
   you can turn computeLPS into an int function and
   rewrite the code without the need for a
   string copy and a separate palindrome checker. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isPalindrome (int start, int end, char *s) {
  if (start >= end) return 1;
  if (s[start] != s[end]) return 0;
  return isPalindrome(start+1, end-1, s);
}

void computeLPS (char *s, char *seq, int n, int idx, 
int subLen, int *maxLen, char *maxSeq) {
  if (idx >= n) {
    if (subLen > *maxLen && isPalindrome(0, subLen-1, seq)) {
      *maxLen = subLen;
      strcpy (maxSeq, seq);
      maxSeq[subLen] = '\0';
    }
    return;
  }
  if (subLen < n) {    // take char at index idx from s
    seq[subLen] = s[idx];    
    computeLPS(s, seq, n, idx+1, subLen+1, maxLen, maxSeq);
  }
  // skip current char from s
  computeLPS(s, seq, n, idx+1, subLen, maxLen, maxSeq);
}


int main(int argc, char **argv){
  char s[] = "kateibdochpuywhetorbilars";
  char seq[50] = {0}, maxSeq[50] = {0};
  int maxLen = 0;
  computeLPS(s, seq, strlen(s), 0, 0, &maxLen, maxSeq);
  printf("Length of the LPS is %d\n", maxLen);
  printf("A possibly non-unique LPS is: %s\n", maxSeq);
  return 0; 
}