____________________________________
*Single-source shortest paths (SSSP)*
____________________________________

# Bellman-Ford algorithm (BFA)

## Problem

Given a graph with $n$ vertices and $m$ edges, find the shortest paths from a given vertex $s$ to all other vertices in the graph.

## Bellman-Ford algorithm

The Bellman-Ford algorithm is an algorithm for finding the shortest paths in a weighted graph which may contain negative edges and negative-weight cycles. It works by repeatedly relaxing the edges of the graph. The running time of this algorithm is $O(n^2 + nm)$, which becomes $O(nm)$ if the graph is dense. The algorithm also detects negative cycles in the graph, in which case the shortest paths are undefined.

Implementation: [Bellman-Ford](https://github.com/pl3onasm/AADS/tree/main/algorithms/graphs/bellman/bfa.c)

## DAG Shortest Paths (DSP)

The Bellman-Ford algorithm can be used to find the shortest paths in a directed acyclic graph (DAG), but it is not the most efficient algorithm for this task. The running time of the algorithm is $O(nm)$, which is worse than the running time of the topological sort algorithm, which is $O(n + m)$. So, if the graph is a DAG, it is better to use the topological sort algorithm to find the shortest paths. This algorithm works by first performing a topological sort on the graph, and then relaxing the edges in the order of the topological sort.

Implementation: [DAG Shortest Paths](https://github.com/pl3onasm/AADS/tree/main/algorithms/graphs/bellman/dsp.c)
