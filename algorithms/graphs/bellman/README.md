____________________________________
*Single-source shortest paths (SSSP)*
____________________________________

## Problem

Given a weighted, directed graph $G = (V,E)$, find the shortest paths from a given vertex $s$ to all other vertices in the graph.

## Bellman-Ford algorithm (BFA)

The Bellman-Ford algorithm is an algorithm for finding the shortest paths in the case where $G$ may contain negative edges and negative-weight cycles. It works by repeatedly relaxing all the edges of the graph. Such a relaxation of an edge $(u,v)$ consists of potentially updating the shortest path estimate and current predecessor of $v$ using $u$ as an intermediate vertex. After $|V|-1$ iterations, the shortest paths are found, and the algorithm concludes by checking for negative-weight cycles. If there are none, the algorithm returns the shortest paths and their weights. Otherwise, the algorithm returns a message that the graph contains a negative-weight cycle, as the shortest paths are undefined in this case.

The check for negative-weight cycles is done by iterating over all the edges and seeing if any distance estimate can still be updated. If there is such an estimate, then that means that there is a negative-weight cycle in the graph. This is because the longest possible path without a cycle has $|V|-1$ edges, and if the first $|V|-1$ iterations of the algorithm did not find a negative-weight cycle, then the $|V|$-th iteration will not update any distance estimates. However, if there is a negative-weight cycle, then the $|V|$-th iteration will update some distance estimates, as the cycle can be repeated as many times as desired.

The running time of this algorithm is $O(V^2 + VE)$, which becomes $O(VE)$ if the graph is dense. This is because the first part dominates the running time, as the algorithm performs $|V|-1$ iterations, and each iteration takes $O(V + E)$ time.

Implementation: [Bellman-Ford](https://github.com/pl3onasm/AADS/tree/main/algorithms/graphs/bellman/bfa.c)

## DAG Shortest Paths (DSP)

The Bellman-Ford algorithm can also be used to find the shortest paths in a weighted, directed *acyclic* graph (DAG), but it is not the most efficient algorithm for this particular task. The DAG shortest paths algorithm is more efficient, as it runs in $O(V+E)$ time, which is faster than the $O(VE)$ time of the Bellman-Ford algorithm.

The algorithm works by first performing a [topological sort](https://github.com/pl3onasm/AADS/tree/main/algorithms/graphs/top-sort) on the vertices, and then relaxing the edges in the linear ordering that was found. As the graph is acyclic, the topological sort is well defined: if edge $(u,v) \in E$, then $(v,u) \notin E$, and $u$ must come before $v$ in the topological sort as $v$ is reachable from $u$ but not vice versa. Recall that a topological sort is a linear ordering of the vertices that preserves any dependencies between them. This means that the shortest path from $s$ to $v$ can indeed be found by relaxing the edges in the order given by the topological sort, since the shortest path from $s$ to any $u$ that precedes $v$ in the graph must be found before the shortest path from $s$ to $v$ can be computed. Once the shortest path to $v$ is found, the algorithm can then move on to the next vertex in the topological sort. Note that a topological sort is not necessarily unique, but any topological sort is guaranteed to work.

Implementation: [DAG Shortest Paths](https://github.com/pl3onasm/AADS/tree/main/algorithms/graphs/bellman/dsp.c)
