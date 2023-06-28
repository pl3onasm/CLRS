# Minimum edit distance (MED)

## Problem

Given two strings, the goal is to transform the source string to the target string using a limited set of operations.

The transformation operations allowed are:

- Copy a character
- Insert a character
- Delete a character
- Replace a character
- Swap two adjacent characters (twiddle)
- Kill the remaining source characters, effectively truncating the source string

Each operation has a given cost associated with it. We want to find the minimum total cost of transforming the source string to the target string and a sequence of operations that achieves this minimum cost.

## DP - Bottom-up

The problem can be solved using dynamic programming. The idea is to build a table where each cell $T[i,j]$ contains the minimum cost of transforming the source string $S[1..i]$ to the target string $T[1..j]$. The table is filled in a bottom-up fashion, starting with the base cases $T[i,0]$ and $T[0,j]$, which are the costs of transforming the empty string to the source and target strings, respectively. The general case is filled by considering the cost of each operation and choosing the one that minimizes the total cost.  
The input consists of the source string $S$ and the target string $T$, and the cost of each operation. The output is the minimum cost $T[m,n]$ and an optimal operation sequence.

Implementation: [MED - DP Bottom-up](https://github.com/pl3onasm/AADS/blob/main/algorithms/dynamic-programming/min-edit-dist/med-1.c)

## Variant: Levenshtein distance

What is known as the Levenshtein distance is actually a simplified variant of the MED problem: it works with only four operations (copy, insert, delete and replace) and the cost of each operation is 1.

Implementation: [MED - Levenshtein variant](https://github.com/pl3onasm/AADS/blob/main/algorithms/dynamic-programming/min-edit-dist/med-2.c)
