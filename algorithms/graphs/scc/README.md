# Strongly connected components (SCC)

## Problem

Given a directed graph, find the strongly connected components of the graph. A strongly connected component of a graph $G = (V,E)$ is a maximal set of vertices $C \subseteq V$ such that every vertex in the set $C$ is reachable from every other vertex in the set.

## Solution

The algorithm to find the strongly connected components of a graph uses its transpose, which is the graph $G^T = (V, E^T)$, where $E^T = \{(u, v) : (v, u) \in E\}$. In other words, the transpose of a graph is the same graph with all its edges reversed.  

The algorithm consists of two steps. First, we run a DFS on the graph $G$ and compute the finish times of each vertex. Then, we run a DFS on the transpose of $G$ in decreasing order of the finish times computed in the first step. Each depth-first tree discovered in this second step forms a strongly connected component of the graph.

To get the list of vertices sorted in decreasing order of their finish times, we can use a topological sort on the graph $G$. All we need to do in the second step is to run a DFS on the transpose of $G$ in the order of the vertices in the sorted list.

The reason why this algorithm works is that the transpose of a graph has the same strongly connected components as the original graph. By running a DFS on the transpose of $G$ in decreasing order of the finish times of $G$, we are guaranteed to explore all vertices in a strongly connected component before moving on to the next one, since the connecting edges between components in $G^T$ are the reverse of the connecting edges in $G$, meaning that any connecting edge in $G^T$ will be a back edge in $G$ pointing to a vertex that has already been explored and will therefore not be explored again. This is why the algorithm returns a depth-first forest in which each tree is indeed a strongly connected component of $G$.

The time complexity of the algorithm is $\Theta(V+E)$, since we need to run a topological sort on the graph, followed by a DFS on the transpose of the graph.

The program below implements the algorithm to find the strongly connected components of a graph. It outputs the vertices in each strongly connected component on a separate line in the order in which they are discovered.

Implementation: [SCC](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/scc/scc.c)
