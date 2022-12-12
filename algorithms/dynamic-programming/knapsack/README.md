# 0-1 Knapsack Problem

We are given a knapsack of a given size and a set of items. Each item has a weight and a value. The goal is to maximize the total value of the items in the knapsack while keeping the total weight less than or equal to the knapsack size. The items can't be used more than once and can't be broken up into smaller pieces: we can either put the whole item in the knapsack (1) or not (0).  
The input to the program is a knapsack of capacity $W$, and two arrays containing n elements: $weights$ and $values$, denoting the weights and values of the items. The output is the maximum value that can be obtained by putting items in the knapsack, as well as the (possibly non-unique) combination of items that yields this value.

## Brute Force

A brute force solution would be to try all possible combinations of items and see which one yields the highest value. This is a recursive solution where in each subproblem we try to put the current item in the knapsack or skip it. If we put it in the knapsack, we subtract the weight of the item from the knapsack size and add the value of the item to the total value. If we skip it, we don't change the knapsack size or the total value. We keep doing this for all subproblems until we have tried all items. The time complexity of this solution is $O(2^n)$ because we have 2 choices for each item and we have n items.

Implementation: [Knapsack - Brute Force](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/knapsack/knapsack-1.c)

## Top-down DP (Memoization)
