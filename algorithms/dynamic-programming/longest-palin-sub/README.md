# Longest Palindrome Subsequence

## Problem

Given a string, find the longest palindromic subsequence. For example, given the string BBABCBCAB, the longest palindromic subsequence is BABCBAB.  

## Brute Force Solution

The brute force solution is to generate all possible subsequences of the string and then check if each subsequence is a palindrome, while keeping track of the longest one. The time complexity of this solution is $O(2_n)$ where n is the length of the string.

Implementation: [LPS - Brute Force](https://github.com/pl3onasm/Algorithms/blob/main/algorithms/dynamic-programming/longest-palin-sub/lps-1.c)

## Dynamic Programming Solution


