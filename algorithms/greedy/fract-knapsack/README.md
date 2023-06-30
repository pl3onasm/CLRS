# Fractional Knapsack Problem (FKP)

## Problem

We are given a set of $n$ items, each with a weight $w_i$ and a value $v_i$. Also given is a knapsack of a fixed capacity $W$. The problem is to find the maximum value subset of the items such that the total weight of the subset is less than or equal to $W$. Unlike the 0-1 knapsack problem, we are allowed to break items (take fractional amounts of items) for maximizing the total value of the knapsack.

## Greedy solution in linearithmic time

This problem can be solved by using a greedy algorithm. The greedy approach is to add items to the knapsack in descending order of their value per unit weight. We keep adding as much of the item as possible without exceeding the knapsack capacity. If we can't add the item in its entirety anymore, we simply add a fraction of it such that the knapsack is filled to capacity.

The greedy choice made at each step is to add the item with the highest value per unit weight. This choice is locally optimal, since it maximizes the value of the knapsack at that step. The algorithm is correct because the items are sorted in decreasing order of their value per unit weight, so the greedy choice made at each step is also globally optimal.

Implementation: [FKP - Linearithmic Greedy](https://github.com/pl3onasm/AADS/tree/main/algorithms/greedy/fract-knapsack/fkp-1.c)  

Note: the greedy approach works for the fractional knapsack problem, but not for the 0-1 knapsack problem. The [0-1 knapsack problem can be solved using dynamic programming](https://github.com/pl3onasm/AADS/tree/main/algorithms/dynamic-programming/knapsack) instead, with a time complexity of $O(nW)$ where $n$ is the number of items and $W$ is the capacity of the knapsack.  

The main reason why the greedy approach works for the fractional knapsack problem is that in this case, the greedy choice made at each step is guaranteed to be globally optimal. This is not the case for the 0-1 knapsack problem, because items can't be broken into fractions so that when you get to the next step, you may find that the greedy choice you made in the previous step was not optimal after all. Thus, when you consider including an item in the knapsack, you are forced to compare the solution of including the item with the solution of not including it before you can safely make a decision that is globally optimal.

## Greedy solution in linear time (CLRS Ex 15.2-6)

The greedy approach described above works, but can be improved upon. The bottleneck of the algorithm is the sorting which runs in $O(n\log n)$ time. However, we can do better by using an algorithm that draws upon the the quickselect algorithm. While quickselect is used to find the $i$-th smallest element in an array, we now use something similar to find the critical pivot that is needed to fill the knapsack to capacity.  
The idea is to partition the items list around a pivot index, such that the items to the left of the pivot index have a higher or equal value per unit weight than the pivot itself. We then compare the sum of the weights of these items to the knapsack capacity.

If the sum equals the capacity, we are done. If the sum is greater than the capacity, that means the total weight of the items to the left of the pivot index exceeds the capacity. In this case, we recurse on the left subarray and keep the capacity the same. Otherwise, we recurse on the right subarray and subtract the sum of the weights of the items to the left of the pivot index from the capacity. We repeat this process until we find a pivot that is critical to filling the knapsack to capacity.  

By a critical pivot we mean that the corresponding weight is the last weight that can be fully added to the knapsack. This means may mean that the knapsack is filled to capacity by simply adding the pivot itself, or that an item with the same value per unit weight as the pivot is the last one to be fully added to the knapsack, in which case that item will be to the left of the pivot index. If there are no items with the same value per unit weight as the pivot, then the pivot itself will be the last item and if needed, the item to the right of the pivot may then be added fractionally to fill the knapsack to capacity.

If we use the [median of medians algorithm](https://github.com/pl3onasm/CLRS-in-C/blob/main/algorithms/divide-and-conquer/quickselect/qselect-2.c) to find an ideal pivot such that the partitioning is always optimally balanced, the time complexity becomes linear in the worst case. Thus, the total time complexity of the algorithm is in $O(n)$.

For simplicity, we use the randomized quickselect algorithm in the implementation. This runs in expected linear time.

Implementation: [FKP - Linear Greedy](https://github.com/pl3onasm/AADS/tree/main/algorithms/greedy/fract-knapsack/fkp-2.c)  
