# Maximum flow problem (Ford-Fulkerson algorithm)

## Problem

Given a directed graph with $m$ non-negative edge weights and a source vertex in the graph, find the maximum flow from this source to a given sink vertex. A *flow* is a mapping from edges to non-negative integers, such that for each vertex $v$ other than the source and the sink, the total flow into $v$ is equal to the total flow out of $v$. The *value* of a flow is the sum of the flows of all edges leaving the source. The *maximum flow* is the flow with the largest value.

## Ford-Fulkerson algorithm

The *Ford-Fulkerson algorithm* is a greedy algorithm that computes a maximum flow in a flow network in $O(mn)$ time, where $m$ is the number of edges and $n$ is the number of vertices. The algorithm is based on the following observation: if there is a path from the source to the sink in the residual graph, then there is a flow that can be pushed along this path without violating the flow conservation constraint. The algorithm repeatedly finds such a path and pushes flow along it until there is no such path. 

The algorithm maintains a residual graph $G_f$ that represents the current flow. Initially, $G_f$ is equal to the original graph $G$ with all flow values equal to zero. The algorithm repeatedly finds a path from the source to the sink in the residual graph $G_f$ and pushes flow along this path until there is no such path. The algorithm terminates when there is no such path. The value of the maximum flow is equal to the total flow out of the source, which is equal to the total flow into the sink.

## Edmonds-Karp algorithm

## Capacity scaling and DFS

## Dinic's algorithm

