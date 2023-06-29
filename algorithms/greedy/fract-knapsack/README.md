# Fractional Knapsack Problem (FKP)

## Problem

We are given a set of $n$ items, each with a weight $w_i$ and a value $v_i$. Also given is a knapsack of a fixed capacity $W$. The problem is to find the maximum value subset of the items such that the total weight of the subset is less than or equal to $W$. Unlike the 0-1 knapsack problem, we are allowed to break items (take fractional amounts of items) for maximizing the total value of the knapsack.

## Greedy solution in linearithmic time

This problem can be solved by using a greedy algorithm. The greedy approach is to add items to the knapsack in descending order of their value per unit weight. We keep adding as much of the item as possible without exceeding the knapsack capacity. If we can't add the item in its entirety anymore, we simply add a fraction of it such that the knapsack is filled to capacity.

The greedy choice made at each step is to add the item with the highest value per unit weight. This choice is locally optimal, since it maximizes the value of the knapsack at that step. The algorithm is correct because the items are sorted in decreasing order of their value per unit weight, so the greedy choice made at each step is also globally optimal.

The implementation uses a priority queue to store the items. The max heap takes $O(n)$ time to build. After that, we can extract the item with the highest value per unit weight in $O(\log n)$ time. This is then repeated until the knapsack is filled to capacity. In the worst case, this takes $n$ iterations, so that the total time complexity is $O(n\log n)$.

Implementation: [FKP - Linearithmic Greedy](https://github.com/pl3onasm/AADS/tree/main/algorithms/greedy/fract-knapsack/fkp-1.c)  

Note: the greedy approach works for the fractional knapsack problem, but not for the 0-1 knapsack problem. The [0-1 knapsack problem can be solved using dynamic programming](https://github.com/pl3onasm/AADS/tree/main/algorithms/dynamic-programming/knapsack) instead, with a time complexity of $O(nW)$ where $n$ is the number of items and $W$ is the capacity of the knapsack.  

The main reason why the greedy approach works for the fractional knapsack problem is that in this case, the greedy choice made at each step is guaranteed to be globally optimal. This is not the case for the 0-1 knapsack problem, because items can't be broken into fractions so that when you get to the next step, you may find that the greedy choice you made in the previous step was not optimal after all. Thus, when you consider including an item in the knapsack, you are forced to compare the solution of including the item with the solution of not including it before you can safely make a decision that is globally optimal.

## Greedy solution in linear time (CLRS Ex 15.2-6)

The greedy approach described above works, but can be improved upon. The bottleneck of the algorithm is the fact that we extract the item with the highest value per unit weight from the priority queue at each step. This takes $O(\log n)$ time per step, so the total time complexity is in $O(n\log n)$. However, we can do better by using quickselect to find the index of the last item to be added fully or partially to the knapsack. This can be done by continually partitioning the items list around the item with the median value per unit weight until we find the index of the item that verges on the knapsack capacity. By then, we know that all the items up to that index can be added to the knapsack in their entirety, and we only need to verify if the next item can be added partially or not. The time complexity of quickselect is in expected linear time, and dominates the time complexity of the algorithm. If we use the [median of medians algorithm](https://github.com/pl3onasm/CLRS-in-C/blob/main/algorithms/divide-and-conquer/quickselect/qselect-2.c) to find the pivot, the time complexity becomes linear in the worst case. Thus, the total time complexity of the algorithm is in $O(n)$.

For simplicity, we use the ordinary quickselect algorithm in the implementation.

Implementation: [FKP - Linear Greedy](https://github.com/pl3onasm/AADS/tree/main/algorithms/greedy/fract-knapsack/fkp-2.c)  
