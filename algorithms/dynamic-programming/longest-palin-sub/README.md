# Longest Palindrome Subsequence

## Problem

Given a string, find the longest palindromic subsequence. For example, given the string BBABCBCAB, the longest palindromic subsequence is BABCBAB.  

## Brute Force Solution

A brute force solution is to generate all possible subsequences of the string and then check if each subsequence is a palindrome, while keeping track of the longest one. This ignores the fact that many subproblems repeat themselves. The time complexity of this solution is $O(2^n)$ where n is the length of the string.

Implementation: [LPS - Brute Force](https://github.com/pl3onasm/Algorithms/blob/main/algorithms/dynamic-programming/longest-palin-sub/lps-1.c)

## Bottom-Up Dynamic Programming Solution

In order to find an LPS, we can break the problem down as follows:

- If the first and last characters of the string are the same, then the LPS consists of the first and last characters plus the LPS of the remaining characters.
- If the first and last characters of the string are different, then the LPS consists of the LPS of the sequence without the first character or the LPS of the sequence without the last character, whichever is longer.
- Keep repeating the above steps until a base case, a subsequence of length 1 or 0, is reached.  

This sounds very similar to the description of the [Longest Common Subsequence Problem](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/longest-common-sub). The only difference seems to be that we are now looking for a palindrome, not a common subsequence. However, asking for a palindrome is the same as asking a question about symmetric self-similarity. In other words, it is the same as asking how similar a given sequence is to its reverse. Once we realize this, the problem is easy to solve. We just need to find the longest common subsequence of the given sequence and its reverse. The time complexity of this solution is the same as the one for the DP solution of the LCS problem, where in this case the length of both strings is the same, so $O(n^2)$.

Implementation: [LPS - Bottom-Up DP](https://github.com/pl3onasm/Algorithms/blob/main/algorithms/dynamic-programming/longest-palin-sub/lps-2.c)
