# Dijkstra's algorithm

## Problem

Given a directed graph with $m$ non-negative edge weights and a source vertex in the graph, find shortest paths from a given source to all the other vertices in the graph. A *shortest path* is a path from the source to a given vertex with the smallest total weight, and the weight of a path is the sum of the weights of its constituent edges.

## Dijkstra's solution

Unlike the Bellman-Ford and the DAG shortest path algorithms, Dijkstra's algorithm doesn't work for graphs with negative weight edges. It is actually a generalization of the BFS algorithm, which also finds shortest paths from a given source vertex to all other vertices, but in an unweighted graph in which all edges have the same unit weight.

The algorithm works by maintaining a set $S$ of vertices whose shortest paths from the source have already been found. Initially, $S$ is empty. At each step, the algorithm finds a vertex that is not in the set $S$, and whose distance from the source is the smallest among all such vertices. It then adds this vertex to $S$, and updates the distances of all other vertices that are adjacent to this vertex. This procedure is repeated until all vertices have been added to the set $S$.

The time complexity using a binary min-heap is $O(m \log n)$, where $m$ is the number of edges and $n$ is the number of vertices. This is fine for sparse graphs, but for dense graphs, the time complexity can be improved to $O(m + n \log n)$ using a Fibonacci heap.

## Implementations

Using a binary min-heap: [binary-dijkstra](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/dijkstra/dijkstra-1.c)

Using a Fibonacci heap: [fibonacci-dijkstra](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/dijkstra/dijkstra-2.c)
