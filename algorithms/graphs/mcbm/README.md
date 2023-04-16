# Maximum Cardinality Bipartite Matching (MCBM)

(Under construction)

## Problem

Given a bipartite graph $G = (R \cup L, E)$, find a maximum cardinality matching $M$. A bipartite graph is a graph whose vertices can be divided into two disjoint vertex sets $R$ and $L$ such that every edge connects a vertex in $R$ to one in $L$. A matching then is a subset of edges $M \subseteq E$ in which no two edges share a common vertex and a maximum cardinality matching is simply a matching that consists of a maximum subset $M$ of edges in $E$.

## Unweighted MCBM (CP 8.5.3)

In this version of the problem, the edges are unweighted. The following algorithms can be used to solve this problem. They are in fact similar but were independently discovered.

### Dinic's Algorithm

We can solve the problem by adapting the maximum flow algorithm. For this, we simply add a super source $s$ and a super sink $t$ to the given graph and then add an edge from $s$ to every vertex in $L$ and an edge from every vertex in $R$ to $t$. The capacity of all edges is set to 1. Next we run Dinic's algorithm on this graph (since it's the fastest max flow algorithm). The resulting maximum flow is then the cardinality of the maximum matching, and the edges with flow are the edges that are part of this matching. The algorithm runs in $O(E \sqrt{V})$ time, thanks to the fact that the graph is unweighted (i.e. all edges have unit capacity) and bipartite, which speeds up the network flow computations.

Implementation: [BCBM - Maxflow](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/mcbm/mcbm-1.c)

### Hopcroft-Karp Algorithm (CLRS⁴ 25.1)

This algorithm actually does the same thing as the previous one, but is presented in a different way. It is based on the idea of alternating paths. We start with an empty matching and we repeatedly find an alternating path from the super source to the super sink. An alternating path is a path that starts from the super source, alternates between edges in the matching and edges not in the matching, and ends at the super sink. We can find such alternating paths using a breadth-first search. If an alternating path is found, we update the matching by adding the edges in the path to the matching $M$ so far and removing the edges not in the path from $M$ (i.e. we take the symmetric difference of the edges in the matching and the edges in the found path). This is done by using a DFS to find the edges in the path and then updating the matching accordingly. This whole process is repeated until we can no longer find an alternating path. The algorithm runs in $O(\sqrt{V} \cdot E)$ time, which is the same as the previous approach.

Implementation: [BCBM - Hopcroft-Karp](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/mcbm/mcbm-2.c)

## Unweighted complete MCBM (CLRS⁴ 25.2)

In this version of the problem, we are given an unweighted complete bipartite graph $G = (R \cup L, E)$, where $|R| = |L| = n$. The fact that the graph is complete means that there is an edge between every pair of vertices in $R$ and $L$. The goal is to find a perfect matching, which is a matching that contains exactly $n$ edges.

## Weighted MCBM

### Min-cost Max-flow Algorithm (CP 9.25)

Implementation: [BCBM - Min-cost max-flow](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/mcbm/mcbm-3.c)

### Hungarian Algorithm (Kuhn-Munkres Algorithm) (CLRS⁴ 25.3)

Implementation: [BCBM - Kuhn-Munkres](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/mcbm/mcbm-4.c)
