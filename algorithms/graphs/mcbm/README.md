# Maximum Cardinality Bipartite Matching (MCBM)

(Under construction)

## Problem

Given a bipartite graph $G = (R \cup L, E)$, find a maximum cardinality matching $M$. A bipartite graph is a graph whose vertices can be divided into two disjoint vertex sets $R$ and $L$ such that every edge connects a vertex in $R$ to one in $L$. A matching then is a subset of edges $M \subseteq E$ in which no two edges share a common vertex and a maximum cardinality matching is simply a matching that consists of a maximum subset $M$ of edges in $E$.

## Unweighted MCBM (CP 8.5.3)

In this version of the problem, the edges are unweighted. The following algorithms can be used to solve this problem. They are in fact similar but were independently discovered.

### Adapted Maximum Flow Algorithm

We can solve the problem by adapting the maximum flow algorithm. For this, we simply add a super source $s$ and a super sink $t$ to the given graph and then add an edge from $s$ to every vertex in $L$ and an edge from every vertex in $R$ to $t$. The capacity of all edges is set to 1. Next we run Dinic's algorithm on this graph (since it's the fastest variant and easier to implement than the relabel-to-front approach). The resulting maximum flow is then the cardinality of the maximum matching, and the edges with flow are the edges that are part of this matching. The algorithm runs in $O(E \sqrt{V})$ time, thanks to the fact that the graph is unweighted (i.e. all edges have unit capacity) and bipartite, which speeds up the network flow computations.

Implementation: [MCBM - Maxflow](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/mcbm/mcbm-1.c)

### Hopcroft-Karp Algorithm (CLRS⁴ 25.1)

This algorithm actually does the same thing as the previous one, but is presented in a different way. It is based on the idea of M-alternating paths. We start with an empty matching and try to repeatedly find an augmenting path (a path that increases the cardinality of the matching).
Such a path is also called an M-alternating path, since it needs to start from an unmatched vertex in $L$ and end at an unmatched vertex in $R$, and needs to alternate between edges in the current matching and edges that are not. These type of paths can be found using a breadth-first search. If any are found, we update the current matching by adding the path edges to $M$ and removing the edges from $M$ that are not in the found paths (i.e. we take the symmetric difference of the edges in the matching and the edges in the found paths). The latter is done by running a DFS: it finds the edges in the paths that were marked by the BFS and then updates the matching $M$ accordingly. This whole process is repeated until no more augmenting paths can be found. The algorithm runs in $O(E \sqrt{V})$ time, which is the same as the previous approach.

Implementation: [MCBM - Hopcroft-Karp](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/mcbm/mcbm-2.c)

## Unweighted complete preferred MCBM (CLRS⁴ 25.2)

In this version of the problem, we are given an unweighted *complete* bipartite graph $G = (R \cup L, E)$. The fact that the graph is complete means that there is an edge from every vertex in $L$ to every vertex in $R$. We are also given a preference list for each vertex in $L$ that specifies the order in which the vertices in $R$ are preferred by the vertex in $L$, and we are given a preference list for each vertex in $R$ that specifies the order in which the vertices in $L$ are preferred by the vertex in $R$. The goal is to find a maximum cardinality matching $M$ such that the preference list of each vertex in $L$ and $R$ is satisfied. A matching is said to be *stable* if there is no better matching that can be found by swapping two edges in $M$, i.e. all preference lists are satisfied. The aim is to find a maximum cardinality stable matching, and this can be done using a greedy algorithm that is known as the Gale-Shapley algorithm.  
It is based on the idea of *proposals* and *rejections*. We start with an empty matching and then we repeatedly propose to each vertex in $L$ the vertex in $R$ that is at the top of its preference list. If the vertex in $R$ is unmatched, then it accepts the proposal and the matching is updated accordingly. If the vertex in $R$ is matched, then it checks if the vertex in $L$ that proposed to it is preferred over the vertex in $L$ that is currently matched to it. If this is the case, then the vertex in $R$ rejects the proposal. If the vertex in $L$ that proposed to it is not preferred over the vertex in $L$ that is currently matched to it, then the vertex in $R$ accepts the proposal and the matching is updated accordingly. This process is repeated until no more proposals can be made and the matching is stable. The algorithm runs in $O(VE)$ time, since each vertex in $L$ proposes to a vertex in $R$ at most once and each vertex in $R$ accepts or rejects a candidate at most once.

Implementation: [MCBM - Gale-Shapley](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/mcbm/mcbm-3.c)

## Weighted MCBM

### Min-cost Max-flow Algorithm (CP 9.25)

Implementation: [MCBM - Min-cost max-flow](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/mcbm/mcbm-3.c)

### Hungarian Algorithm (Kuhn-Munkres Algorithm) (CLRS⁴ 25.3)

Implementation: [MCBM - Kuhn-Munkres](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/mcbm/mcbm-4.c)
