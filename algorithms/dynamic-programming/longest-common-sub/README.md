# Longest Common Subsequence (LCS)

## Problem

Given two sequences $X = (x_1, x_2, ..., x_m)$ and $Y = (y_1, y_2, ..., y_n)$, find the length of their longest common subsequence (LCS), i.e. the longest subsequence that is present in both sequences. A subsequence of a given sequence is derived from that sequence by deleting some elements (possibly none) without changing the order of the remaining elements. For example, the sequence $(A, B, D)$ is a subsequence of $(A, B, C, D, B, A)$.  
In other words, the problem is to find the length of the longest subsequence $Z = (z_1, z_2, ..., z_k)$ such that $z_i = x_j$ and $z_i = y_h$ for each $1 \leq i \leq k$ and some $1 \leq j \leq m$ and $1 \leq h \leq n$. For example, the longest common subsequence of the sequences $X = (A, B, C, B, D, A, B)$ and $Y = (B, D, C, A, B, A)$ is $Z = (B, C, A, B)$, which has length $4$. Mind that the longest common subsequence is not necessarily unique: $Z = (B, D, A, B)$ is also a valid solution.

The idea is to start from the end of the sequences $X$ and $Y$ and compare their last elements, $x_m$ and $y_n$:

- If they are equal, then this identical element is necessarily part of the LCS $Z$. So we add it to $Z$, and move on to establishing an LCS for the remaining elements of $X$ and $Y$, i.e. an LCS for the sequences $X_{1:m-1}$ and $Y_{1:n-1}$.
- If they are not equal, then we can either remove the last element of $X$ and establish an LCS for $X_{1:m-1}$ and $Y$, or we can remove the last element of $Y$ and establish an LCS for $X$ and $Y_{1:n-1}$. Both of these options should be considered, and the longest LCS should be chosen.

As we keep repeating this process for the remaining elements of $X$ and $Y$, we continually reduce the size of the problem to smaller subproblems, i.e. to smaller prefixes of $X$ and $Y$. Eventually, we will reach the base case of an empty sequence, when we reach the beginning of one of the original sequences, at which point the LCS $Z$ is fully computed for a particular prefix of $X$ and $Y$ (or for the entire sequences $X$ and $Y$) and we can return it.

## Brute Force Solution

This is a straightforward recursive implementation of the idea described above. It is exponential in the length of the input sequences, since it computes the LCS for all possible prefixes of $X$ and $Y$, ignoring the fact that some of these prefixes are repeated. The time complexity is in $O(2^{m+n})$.

Implementation: [LCS - Brute Force](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/longest-common-sub/lcs-1.c)

The code can be simplified and looks more natural if we start from the beginning of the sequences $X$ and $Y$ instead of the end. The idea is the same, but the subproblems are now defined for the suffixes of $X$ and $Y$ instead of the prefixes, and the base case is when the suffixes are empty. The time complexity is still exponential. For the dynamic programming solutions, I will use the original definition of the subproblems, i.e. for the prefixes of $X$ and $Y$, in order to keep in line with how things are presented in the textbook by Cormen et al.

Implementation: [LCS - Brute Force - suffixes](https://github.com/pl3onasm/Algorithms/blob/main/algorithms/dynamic-programming/longest-common-sub/lcs-2.c)

## Top-down Approach with Memoization

This approach maintains the top-down strategy of the previous solution, but it uses a memoization table to avoid recomputing the LCS for the same prefixes of $X$ and $Y$ multiple times. The time complexity is in $O(mn)$. The space complexity is in $O(mn)$, since the memoization table has $m$ rows and $n$ columns. If we don't want to reconstruct the LCS, we can use a 2D array of size $2 \times n$ instead of a 2D array of size $m \times n$, since we only need the previous row of the memoization table to compute the next row.

Implementation: [LCS - Top-Down DP](https://github.com/pl3onasm/Algorithms/blob/main/algorithms/dynamic-programming/longest-common-sub/lcs-3.c)

## Bottom-up Approach

Here we use a bottom-up approach, which is more natural for dynamic programming. We start from the base case of an empty sequence, and we compute the LCS for all possible prefixes of $X$ and $Y$ in a bottom-up fashion. Both the time and space complexity are in $O(mn)$. The same space optimization as in the previous solution can be applied here.

Implementation: [LCS - Bottom-up DP](https://github.com/pl3onasm/Algorithms/blob/main/algorithms/dynamic-programming/longest-common-sub/lcs-4.c)

## Notes

The implementations can of course be simplified if you only want to return the length of the LCS, but I found it more interesting to return both the length of the LCS and an actual LCS. It should be an easy exercise to extend the code so that it returns all possible LCSs if there are multiple of the same maximum length.  
