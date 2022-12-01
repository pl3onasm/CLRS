# Rod Cutting

## Problem

Given a rod of length $n$ inches and a table of prices $p_i$ for $i = 1, 2, ..., n$, determine the maximum revenue $r_n$ obtainable by cutting up the rod and selling the pieces. Note that if the price $p_n$ for a rod of length $n$ is large enough, an optimal solution may require no cutting at all.

## Recursive Solution

The recursive solution is straightforward. We can cut the rod into two pieces: one of length $i$ and the other of length $n - i$. The revenue is then $p_i + r_{n-i}$. We can find the maximum revenue by trying all possible values of $i$ and taking the maximum, thereby ignoring the fact that the same subproblem may be encountered multiple times during the recursive calls. The running time of this approach is in $O(2^n)$.

Implementation: [Naive recursive solution](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/rod-cutting/cut-rod1.c)

## Top-down Approach with Memoization

The recursive solution has overlapping subproblems. We can use memoization to store the results of subproblems and thus avoid recomputing them. This way, we can solve the problem in $O(n^2)$ time.

Implementation: [Memoized solution](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/rod-cutting/cut-rod2.c)

## Bottom-up Approach

The bottom-up approach solves the subproblems in order of increasing piece size, while storing the solution to each subproblem when it is first solved. Thus, the maximum revenues for subproblems of size $1, 2, ..., n$ are computed in ascending order, and all required partial solutions are readily available to solve the next subproblem in line. The solution to the original problem is then the solution to the subproblem of size $n$. The running time of this approach is also in $O(n^2)$. Note that the bottom-up approach requires a double loop, whereas the top-down approach only needs a single loop, since it uses recursion instead.

Implementation: [Bottom-up solution](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/rod-cutting/cut-rod3.c)

## Extended Bottom-up Approach

The extended bottom-up approach not only computes the maximum revenue, but also the optimal solution: a list of piece sizes. For this purpose, an array is used to store the optimal first cut for each subproblem.

Implementation: [Extended bottom-up solution](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/rod-cutting/cut-rod4.c)
