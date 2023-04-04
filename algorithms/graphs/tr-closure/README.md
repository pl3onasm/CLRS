# Transitive closure of a graph (TCG)

## Problem

Given a directed graph $G$ with m edges and n vertices, find the transitive closure of the graph, i.e. a matrix $M$ that indicates whether there is a path from vertex $i$ to vertex $j$ in the graph. In other words, $m_{ij}$ should show whether $j$ is reachable from $i$ or not.

## Solution using Floyd-Warshall algorithm

We can obtain the transitive closure of a graph by running the Floyd-Warshall algorithm on the given graph. If there is a path from $i$ to $j$, then the distance between $i$ and $j$ will be finite. Otherwise, the distance will be infinite. We can even ignore the weights of the edges in the graph and assume that all the edges have weight 1. In that case, there is a path from $i$ to $j$ iff the distance between $i$ and $j$ is less than n. The result is a matrix $M$ where $m_{ij}$ is either infinite if there is no path from $i$ to $j$ or finite with the number of edges needed to go from $i$ to $j$.

Time complexity: $O(V^3)$

Implementation: [Transitive Closure - FW](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/tr-closure/tcg-1.c)

## Slightly optimized solution

A small optimization of the previous method is to substitute the operations $\textit{min}$ and $+$ with the logical operations $\lor$ and $\land$ in the Floyd-Warshall algorithm. The result is then a matrix $M$ where $m_{ij}$ is either 0 if there is no path from $i$ to $j$ or 1 if there is a path from $i$ to $j$.

Time complexity: $O(V^3)$

Implementation: [Transitive Closure - Optimized](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/tr-closure/tcg-2.c)