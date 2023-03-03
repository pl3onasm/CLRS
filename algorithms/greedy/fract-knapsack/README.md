# Fractional Knapsack Problem (FKP)

## Problem

We are given a set of $n$ items, each with a weight $w_i$ and a value $v_i$. Also given is a knapsack of a fixed capacity $W$. The problem is to find the maximum value subset of the items such that the total weight of the subset is less than or equal to $W$. Unlike the 0-1 knapsack problem, we are allowed to break items (take fractional amounts of items) for maximizing the total value of the knapsack.

## Greedy solution in linearithmic time

This problem can be solved by using a greedy algorithm. First, the items need to be sorted in decreasing order of their value per unit weight. Then we add items to the knapsack one by one, starting with the item with the highest value per unit weight. We add as much of the item as possible without exceeding the knapsack capacity. If we can't add the item in its entirety, we simply add a fraction of it such that the knapsack is filled to capacity.

The greedy choice made at each step is to add the item with the highest value per unit weight. This choice is locally optimal, since it maximizes the value of the knapsack at that step. The algorithm is correct because the items are sorted in decreasing order of their value per unit weight, so the greedy choice made at each step is also globally optimal.

Implementation: [FKP - Linearithmic Greedy](https://github.com/pl3onasm/AADS/tree/main/algorithms/greedy/fract-knapsack/fkp-1.c)  

Time complexity: $O(n\log n)$, due to the sorting step.

Note: the greedy approach works for the fractional knapsack problem, but not for the 0-1 knapsack problem. The [0-1 knapsack problem can be solved using dynamic programming](https://github.com/pl3onasm/AADS/tree/main/algorithms/dynamic-programming/knapsack) instead, with a time complexity of $O(nW)$ where $n$ is the number of items and $W$ is the capacity of the knapsack.  

The main reason why the greedy approach works for the fractional knapsack problem is that in this case, the greedy choice made at each step is also globally optimal. This is not the case for the 0-1 knapsack problem, because items can't be broken into fractions and thus the greedy choice for one step may not be optimal for the next step.

## Greedy solution in linear time (CLRS Ex 15.2-6)

The greedy approach described above works, but can be improved upon. The bottleneck of the algorithm is the sorting step, which takes $O(n\log n)$ time. We can improve the time complexity to $O(n)$ by using a priority queue to store the items. This can be implemented using a max heap, which takes $O(n)$ time to build. After that, we can extract the item with the highest value per unit weight in $O(\log n)$ time. This is then repeated until the knapsack is filled to capacity.

Implementation: [FKP - Linear Greedy](https://github.com/pl3onasm/AADS/tree/main/algorithms/greedy/fract-knapsack/fkp-2.c)
