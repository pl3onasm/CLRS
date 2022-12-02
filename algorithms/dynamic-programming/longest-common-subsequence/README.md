# Longest Common Subsequence

## Problem

Given two sequences $X = (x_1, x_2, ..., x_m)$ and $Y = (y_1, y_2, ..., y_n)$, find the length of their longest common subsequence (LCS), i.e. the longest subsequence that is present in both sequences. A subsequence of a given sequence is derived from that sequence by deleting some elements (possibly none) without changing the order of the remaining elements. For example, the sequence $(A, B, D)$ is a subsequence of $(A, B, C, D, B, A)$.  
In other words, the problem is to find the length of the longest subsequence $Z = (z_1, z_2, ..., z_k)$ such that $z_i = x_j$ and $z_i = y_h$ for each $1 \leq i \leq k$ and some $1 \leq j \leq m$ and $1 \leq h \leq n$. For example, the longest common subsequence of the sequences $X = (A, B, C, B, D, A, B)$ and $Y = (B, D, C, A, B, A)$ is $Z = (B, C, A, B)$, which has length $4$. Mind that the longest common subsequence is not necessarily unique: $Z = (B, D, A, B)$ is also a valid solution.

The idea is to start from the end of the sequences $X$ and $Y$ and compare their last elements, $x_m$ and $y_n$. 

- If they are equal, then this identical element is necessarily part of the LCS $Z$. So we add it to $Z$, and move on to establishing an LCS for the remaining elements of $X$ and $Y$, i.e. an LCS for the sequences $X_{1:m-1}$ and $Y_{1:n-1}$.
- If they are not equal, then we can either remove the last element of $X$ and establish an LCS for $X_{1:m-1}$ and $Y$, or we can remove the last element of $Y$ and establish an LCS for $X$ and $Y_{1:n-1}$. Both of these options should be considered, and the longer LCS should be chosen.  

As we keep repeating this process for the remaining elements of $X$ and $Y$, we continuously reduce the size of the problem to smaller subproblems, i.e. to smaller prefixes of $X$ and $Y$. Eventually, we will reach the base case of an empty sequence, when we reach the beginning of one of the original sequences, at which point the LCS $Z$ is fully computed.

## Brute Force Solution

This is a straightforward recursive implementation of the idea described above. It is exponential in the length of the input sequences, since it computes the LCS for all possible prefixes of $X$ and $Y$, ignoring the fact that some of these prefixes are repeated. The time complexity is in $O(2^{m+n})$.

Implementation: [Naive recursive solution](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/longest-common-subsequence/lcs-1.c)

## Top-down Approach with Memoization


## Bottom-up Approach

