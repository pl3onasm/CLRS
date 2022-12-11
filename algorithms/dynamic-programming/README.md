# Dynamic Programming

Dynamic programming is most suited for optimization problems. Such problems should have two main properties in order to apply dynamic programming:

1. <ins>Optimal substructure</ins>  
   The optimal solution to a problem consists of optimal solutions to its subproblems. Mind that the subproblems should be *independent* of each other, i.e. the optimal solution to one subproblem should not depend on the optimal solution to another subproblem.
2. <ins>Overlapping subproblems</ins>  
   Subproblems share subsubproblems, and the same subsubproblems are encountered multiple times during the solution of a subproblem. Dynamic programming takes advantage of this property by solving each subsubproblem only once and then storing the solution in a table where it can be looked up when needed.  
   A good test for this property is to see if the recursive solution has repeated calls for the same inputs. This is different from the type of subproblems that divide-and-conquer algorithms solve, where recursive calls create different subproblems each time.

## Problems

* [Rod Cutting](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/rod-cutting)
* [Matrix Chain Multiplication](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/matrix-chain-mult)
* [Longest Common Subsequence](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/longest-common-sub)
* [Longest Palindromic Subsequence](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/longest-palin-sub)
* [Longest Increasing Subsequence](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/longest-increasing-sub)
* [0-1 Knapsack Problem](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/knapsack)
* Minimal Palindromic Partitioning
