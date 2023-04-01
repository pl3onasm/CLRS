# Minimum spanning trees (MST)

## Problem

Given a connected, undirected graph with $n$ vertices and $m$ edges, each with positive edge weights, find a spanning tree with minimum total weight. A *spanning tree* is a subset of $n - 1$ edges that forms a tree (i.e. an acyclic connected graph) that includes every vertex. The weight of a spanning tree is the sum of the weights of its edges. Note that a spanning tree always has $n - 1$ edges. A *minimum* spanning tree is just a spanning tree with minimum weight. Also note that a graph may have more than one minimum spanning tree.

## Kruskal's algorithm

In Kruskal's algorithm, we start with an empty spanning tree. We then add the edges of the graph, one by one, in increasing order of their weights. If adding an edge would create a cycle, we do not add it. This procedure thus results in a minimum spanning tree. The running time of this algorithm is $O(m \log m)$, since we need to sort the edges in increasing order of their weights, and we need to perform $m$ operations on a disjoint-set data structure, each of which takes $O(\alpha(n))$ time, where $\alpha(n)$ is the inverse Ackermann function. This is a very slow function, but it is still bounded by a constant, so we can ignore it. Thus, the running time of Kruskal's algorithm is $O(m \log m)$.  
In order to implement Kruskal's algorithm, we need a data structure that can efficiently perform the following operations:

* Find the set that a given element belongs to
* Merge two sets into one
* Create a new set containing a single element

The most efficient data structure for this is the *disjoint-set data structure*. We can implement this data structure using a *union-find* data structure. This data structure is a forest of trees, where each tree represents a set. Each node in the tree has a pointer to its parent, and a rank. The rank of a node is the length of the longest path from the node to a leaf. The root of a tree has rank 0. The union operation is performed by merging the trees with the smaller rank into the tree with the larger rank. The find operation is performed by following the parent pointers until we reach a node whose parent is itself. This node is the root of the tree, and thus the representative element of the set.  
The running time of this data structure is better than that of an implementation for disjoint sets using linked lists, thanks to the use of two heuristics: *path compression* and *union by rank*. Path compression is performed by making each node in the path from the node to the root point directly to the root. Union by rank is performed by merging the tree with the smaller rank into the tree with the larger rank, and adding 1 to the rank of the resulting tree if the ranks are equal.

For a detailed explanation of the union-find data structure and the heuristics, see CLRS³, chapter 21.3, or CLRS⁴, chapter 19.3.

Implementation: [MST - Kruskal](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/mst/mst-1.c)

## Prim's algorithm

In Prim's algorithm, we don't work with growing sets, but with growing (connected) trees. We start with a tree containing a single vertex, and we add edges to this tree one by one. At each step, we add the edge with the smallest weight that connects a vertex in the tree to a vertex not in the tree. This procedure results in a minimum spanning tree. Using a binary min-heap, the running time of this algorithm is $O(m \log n)$, since we need to perform $m$ operations on a binary min-heap, each of which takes $O(\log n)$ time, which in the case of dense graphs
dominates the number of extract-min operations, which take $O(n \log n)$ time in total.

Implementation: [MST - binary Prim](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/mst/mst-2.c)

We can also use a Fibonacci heap to implement Prim's algorithm. This will result in a faster algorithm, since the Fibonacci heap has a better amortized running time than the binary min-heap. For more information on Fibonacci heaps, see CLRS³, chapter 19. Unfortunately, they were omitted from CLRS⁴. The running time of this version of the algorithm is $O(m + n \log n)$, which is a significant improvement over the $O(m \log n)$ running time of the binary heap version in the case of dense graphs.

Implementation: [MST - Fibonacci Prim](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/mst/mst-3.c)
