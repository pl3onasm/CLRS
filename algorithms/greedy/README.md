# Greedy algorithms

## Introduction

Like dynamic programming, greedy algorithms are best suited to optimization problems. Unlike dynamic programming, which is based on breaking down a problem into subproblems and then combining the solutions to the subproblems in order to solve the original problem, greedy algorithms make locally optimal choices at each stage with the hope of finding a global optimum. A greedy algorithm can also be used as an approximation algorithm for a problem.  

There are two key properties of a problem that suggest the use of a greedy algorithm:

1. **Greedy choice property**  
   A global optimum can be reached by making the greedy choice at each stage. That is, at each step, we make the choice that looks best at that moment, without looking ahead to see whether it is the best choice or without depending on solutions to subproblems (this is the key difference with dynamic programming). In other words, each choice is made locally, without considering the global effect of that choice.

2. **Optimal substructure**  
   A problem has optimal substructure if an optimal solution can be constructed from optimal solutions to its subproblems. This is a key indredient for both dynamic programming and greedy algorithms. In the case of greedy algorithms, each greedy choice leads to a smaller subproblem, and we need to show that an optimal solution to the subproblem, combined with the greedy choice that led to the subproblem, yields an optimal solution to the original problem.  

## Problems

|**CLRS** | **Link** |
|:---:|:---|
| Chap 15.1 | [Activity-selection Problem](https://github.com/pl3onasm/AADS/tree/main/algorithms/greedy/activity-selection)
| Chap 15.2 | [Fractional Knapsack](https://github.com/pl3onasm/AADS/tree/main/algorithms/greedy/fract-knapsack)
| Chap 15.3 | [Huffman Codes](https://github.com/pl3onasm/AADS/tree/main/algorithms/greedy/huffman)
| Prob 15-1 | [Coin Changing](https://github.com/pl3onasm/AADS/tree/main/algorithms/greedy/coin-changing)
