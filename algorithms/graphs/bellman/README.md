# Bellman-Ford algorithm (BFA)

## Problem

Given a graph with $n$ vertices and $m$ edges, find the shortest paths from a given vertex $s$ to all other vertices in the graph. The graph may contain negative-weight edges.

## Algorithm

The Bellman-Ford algorithm works by repeatedly relaxing the edges of the graph. The running time of this algorithm is $O(n^2 + nm)$, which becomes $O(nm)$ if the graph is dense. The algorithm also detects negative cycles in the graph, in which case the shortest paths are undefined.

Implementation: [Bellman-Ford](https://github.com/pl3onasm/AADS/tree/main/algorithms/graphs/bellman/bfa.c)
