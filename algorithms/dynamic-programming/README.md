# Dynamic Programming

Dynamic programming is most suited for optimization problems. Such a problem should have two main properties in order to apply dynamic programming:

1. <ins>Optimal substructure</ins>  
   The optimal solution to a problem consists of optimal solutions to its subproblems. Mind that the subproblems should be *independent* of each other, i.e. the optimal solution to one subproblem should not depend on the optimal solution to another subproblem.
2. <ins>Overlapping subproblems</ins>  
   Subproblems share subsubproblems, and the same subsubproblems are encountered multiple times during the solution of a subproblem. Dynamic programming takes advantage of this this property by solving each subsubproblem once and then storing the solution in a table where it can be looked up when needed.

## Problems

* [Rod Cutting](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/rod-cutting)
* [Matrix Chain Multiplication](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/matrix-chain-multiplication)
* Longest Common Subsequence
* Longest Increasing Subsequence
* Longest Palindromic Subsequence
* Knapsack problem
* Floyd-Warshall algorithm
  