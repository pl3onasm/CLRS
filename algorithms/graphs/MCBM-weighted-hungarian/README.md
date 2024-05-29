_______________________________________________
*Maximum cardinality bipartite matching (MCBM)*
_______________________________________________

## Problem

Given a bipartite graph $G = (R \cup L, E)$, find a maximum cardinality matching $M$. A bipartite graph is a graph whose vertices can be divided into two disjoint vertex sets $R$ and $L$ such that every edge connects a vertex in $R$ to one in $L$. A matching then is a subset of edges $M \subseteq E$ in which no two edges share a common vertex and a maximum cardinality matching is simply a matching that consists of a maximum subset $M$ of edges in $E$.

In this version of the problem, the edges come with a cost and we are asked to find a maximum cardinality matching $M$ such that the total cost of the edges in $M$ is minimized or maximized.

### Hungarian Algorithm (Kuhn-Munkres Algorithm) (CLRS⁴ 25.3)

This implementation follows the pseudo code as closely as possible, while also implementing the optimization presented in problem 25-2 of the book CLRS⁴. This optimization allows the algorithm to run in $O(V^3)$ time, and involves the introduction of a slack attribute for each vertex in the graph (CLRS calls it the $\sigma$ attribute, but following the general convention in the literature, we will call it the slack attribute instead).
Also, since the label of a vertex already indicates its name, we will use the term height for what CLRS calls the label of a vertex.

Implementation: [MCBM - Kuhn-Munkres](hungarian.c)