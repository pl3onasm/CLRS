# Activity-selection problem

## Problem

Given is a set of $n$ competing activities $A = \{a_1, a_2, \dots, a_n\}$, where each activity $a_i$ is defined by a pair of integers $(s_i, f_i)$, where $s_i$ is the start time and $f_i$ is the finish time. The problem is to select a maximum-size subset of mutually compatible activities. Two activities $a_i$ and $a_j$ are compatible if the intervals $[s_i, f_i)$ and $[s_j, f_j)$ don't overlap, i.e. $s_j \geq f_i$ or $s_i \geq f_j$. We assume that the activities are already sorted by their finish time $f_i$.

## DP solution

The problem can be solved using dynamic programming, since it exhibits optimal substructure, and the subproblems overlap.

- The optimal subctructure resides in the fact that an optimal solution to the entire problem consists of optimal solutions to its subproblems. If we select a certain activity to be included in the solution, then the problem is split into two subproblems: the set of activities that start before the selected activity, and the set of activities that start after the selected activity, and the optimal solution to the entire problem is then the union of the optimal solutions to the two subproblems, plus the selected activity.
- The overlap comes from the fact that we don't know a priori which activity to select in order to get the optimal solution, so we have to try all of them, and the subproblems are not independent, since they share the same overall set of activities. In other words, we have to solve the same subproblems multiple times.

If we consider the set of activities $A = \{a_1, a_2, \dots, a_n\}$, we can choose to include activity $a_k$ in the solution, or not. If we do, we split the problem into two subproblems: the set of activities that start before $a_k$, and the set of activities that start after $a_k$, and then continue to recursively solve the two subproblems.  
If we denote a subset of activities having start times after activity $a_i$ and finish times before activity $a_j$ by $A_{ij}$, and let $s[i,j]$ be the size of the optimal solution to the problem of selecting a maximum-size subset of mutually compatible activities from the set $A_{ij}$, then we have the following recurrence relation:

$$
s[i,j] =  
\begin{cases}
0 & \text{if } A_{ij} = \emptyset \\
\max\{s[i,k] + s[k,j] + 1 \} & \text{if } A_{ij} \neq \emptyset
\end{cases}
$$

where $k$ is the index of the activity that we choose to include in the solution. As we noted above, we need to try all possible values of $k$ in order to find the optimal solution.

Implementation using bottom-up DP: [ASP - Bottom-up DP]()

Note the similarity with the DP implementation for the matrix-chain multiplication problem. For the activity-selection problem, we have to add two sentinel activities, one with start time $0$ and finish time $0$, and another one with start time $\infty$ and finish time $\infty$, in order to simplify the implementation.

## Greedy solution

