____________________________________
*Single-source shortest paths (SSSP)*
____________________________________

## Problem

Given a weighted, directed acyclic graph $G = (V,E)$, which may contain negative-weight edges, and a source vertex $s \in V$, find the shortest paths from $s$ to all other vertices in the graph.

## DAG Shortest Paths (DSP)

The [Bellman-Ford algorithm](https://github.com/pl3onasm/AADS/tree/main/algorithms/graphs/SSSP-bellman) can also be used to find the shortest paths in a weighted, directed *acyclic* graph (DAG), but it is not the most efficient algorithm for this particular task. The DAG shortest paths algorithm is more efficient, as it runs in $O(V+E)$ time, which is faster than the $O(VE)$ time of the Bellman-Ford algorithm.

The algorithm works by first performing a [topological sort](https://github.com/pl3onasm/AADS/tree/main/algorithms/graphs/top-sort) on the vertices, and then relaxing the edges in the linear ordering that was found. As the graph is acyclic, the topological sort is well defined: if edge $(u,v) \in E$, then $(v,u) \notin E$, and $u$ must come before $v$ in the topological sort as $v$ is reachable from $u$ but not vice versa. Recall that a topological sort is a linear ordering of the vertices that preserves any dependencies between them. This means that the shortest path from $s$ to $v$ can indeed be found by relaxing the edges in the order given by the topological sort, since the shortest path from $s$ to any $u$ that precedes $v$ in the graph must be found before the shortest path from $s$ to $v$ can be computed. Once the shortest path to $v$ is found, the algorithm can then move on to the next vertex in the topological sort. In this way, the algorithm only needs to perform $|V|$ relaxations, where each relaxation takes $O(1)$ time, so that the total running time is $O(V+E)$. Note that a topological sort is not necessarily unique, but any topological sort is guaranteed to work for this algorithm.

Implementation: [DAG Shortest Paths](https://github.com/pl3onasm/AADS/tree/main/algorithms/graphs/SSSP-DAG/dsp.c)
