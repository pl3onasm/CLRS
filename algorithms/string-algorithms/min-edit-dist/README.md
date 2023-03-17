# Minimum edit distance (MED)

## Problem

Given two strings, find the minimum number of operations required to convert one string to another. The operations allowed are:

- Copy a character
- Insert a character
- Delete a character
- Replace a character
- Swap two adjacent characters (twiddle)
- Kill the remaining source characters

## DP - Bottom-up

Implementation: [MED - DP Bottom-up](https://github.com/pl3onasm/AADS/blob/main/algorithms/string-algorithms/min-edit-dist/med-1.c)

## Variant: Levenshtein distance

What is known as the Levenshtein distance is actually a simplified variant of the MED problem: it works with only four operations (copy, insert, delete and replace) and the cost of each operation is 1. The Levenshtein distance is also known as the edit distance.

Implementation: [MED - Levenshtein variant](https://github.com/pl3onasm/AADS/blob/main/algorithms/string-algorithms/min-edit-dist/med-2.c)
