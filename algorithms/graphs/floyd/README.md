_________________________________________
*All-Pairs Shortest Paths Problem (APSP)*
_________________________________________

# Problem

Given a weighted graph $G$ with $m$ edges and $n$ vertices, find the shortest paths (i.e. the minimum-weight paths) between all pairs of vertices. The weight of a path is the sum of the weights of its constituent edges.

## Bottom-Up DP Solution

First of all, observe that any shortest path consists of at most $r = n-1$ edges. Also note that the shortest path $p$ between any two vertices $i$ and $j$ can be decomposed into a shortest subpath $p_1$ from $i$ to some vertex $k$, plus one more edge from $k$ to $j$. This decomposition can be repeated $r$ times for all $r = 1, 2, \ldots, n-1$, giving us a shortest path $p$ with $r$ edges.  
If we denote the weight of the shortest path between $i$ and $j$ using at most $r$ edges by $d_{ij}^{(r)}$, then $d_{ij}^{(0)}$ is the weight of the shortest path between $i$ and $j$ using at most $0$ edges, which is $0$ if $i = j$ and $\infty$ otherwise. From this base case, we can then extend paths by adding one more edge at each step, until we reach the desired number of edges $r = n-1$. The weight of the shortest path between any two vertices $i$ and $j$ is then given by the minimum between the weight of the shortest path between $i$ and $j$ using at most $r$ edges, and the shortest path between $i$ and $k$ using at most $r-1$ edges. We thus get the following recurrence:

$$
d_{ij}^{(r)} = \min \lbrace d_{ik}^{(r-1)} + w_{kj} : 1 \leq k \leq n \rbrace
$$

The solution after $n-1$ iterations is then $d_{ij}^{(n-1)} = \delta_{ij}$, which is the weight of the shortest path between any two vertices $i$ and $j$. Note that for $k = 1$, we have $D^{1} = W$, which is the adjacency matrix of the graph, where each entry $w_{ij}$ equals the weight of the edge $(i, j)$, or $\infty$ if there is no edge between $i$ and $j$.

The resulting procedure for computing the final distance matrix $D^{n-1}$, containing all the weights of the shortest paths between any two vertices $i$ and $j$, resembles the standard matrix multiplication algorithm, but iterated $n-1$ times (once for each additional edge in the paths) and instead of taking the sum of the products of the corresponding entries, we take for each entry $d_{ij}$ the minimum between the previous path weight and the weight of each possible extension of the path by one edge. This correspondence with the matrix multiplication algorithm leads to the observation that the distance matrix $D$ is in fact the $n$-th power of the adjacency matrix $W$ of the graph, i.e. $D = W^n$. Thus, the time complexity of the entire algorithm is $\Theta(n^4)$, since we have to repeat $n-1$ times a procedure that takes $\Theta(n^3)$ time (due to the three nested loops in the matrix multiplication algorithm).

If additionally we want to know the actual vertices lying on the shortest paths between any two vertices $i$ and $j$, we also need to keep track of the consecutive choices for the vertex $k$ in order to be able to reconstruct those paths. This means we need to compute an extra predecessor matrix $P$, where $p_{ij}$ equals vertex number $k$ such that $i \leadsto k \to j$ is the shortest path between $i$ and $j$.

Implementation: [APSP - Bottom-Up DP](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/floyd/apsp-1.c)

**Note:** The pseudocode in the textbook is needlessly cumbersome, since by inserting a simple if statement in the path extension procedure (to check if the relevant edges are actually in the graph), we can keep using the same matrix $D$ for each iteration and update it in place.

## Optimized Bottom-Up DP Solution

We can optimize the above solution, which tries to extend paths by one edge at a time. The above observation that the distance matrix $D$ actually is the $n$-th power of the adjacency matrix $W$, leads to the idea that we can use the technique of repeated squaring to compute the $n$-th power of $W$ in $\Theta(n^3 \log n)$ time, by multiplying it $\lceil \log_2 (n-1) \rceil$ times by the intermediate powers of itself, instead of computing it naively by multiplying it $n-1$ times by the original adjacency matrix $W$. For this, we initialize the distance matrix $D$ to $W$, and then repeatedly extend paths by powers of two, i.e. "multiply" the distance matrix $D$ by itself, until we get $D^{n-1}$, which is the distance matrix we want.  

Note that the distance matrix $D$ will not change anymore after $n-1$ iterations, so that we can stop after *any* $r \geq n-1$ and get the same matrix $D^{n-1}$, meaning we can also conveniently stop after $r = 2^{\lceil \log_2 (n-1) \rceil} = 2^{\lceil \log_2 (n) \rceil - 1}$ iterations, and get the same result, but now in $\Theta(n^3 \log n)$ time. The predecessor matrix can be computed in the same time; we just need to initialize it differently (so that it corresponds to the initial distance matrix $D$ = $W$), and then we can also repeatedly square it, until we get $P^{n-1}$.

Implementation: [APSP - Optimized Bottom-Up DP](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/floyd/apsp-2.c)

## Floyd-Warshall Algorithm

The Floyd-Warshall algorithm is also a bottom-up dynamic programming solution to the same problem, but instead of gradually increasing the length of the paths by adding one edge at a time, it tries to find a shorter path between any two vertices $i$ and $j$ by adding one intermediate vertex $k$ at a time. In other words, instead of gradually increasing the upper bound on the number of edges in the shortest paths between any two vertices $i$ and $j$, it gradually increases the upper bound on the number of intermediate vertices in the shortest paths. This is done by trying to find a path between $i$ and $j$ that goes through $k$. If such a path exists, then the distance between $i$ and $j$ is updated to the minimum of the mimimum weight so far, and the weight of the path through $k$. This is done for all pairs of vertices $i$ and $j$, until we have tried all possible values of $k$.

This leads to the following recurrence relation for the shortest path between any two vertices $i$ and $j$ using at most $k$ intermediate vertices:

$$
d_{ij}^{(k)} =  
\begin{cases}
w_{ij} & \text{if } k = 0 \\
\min \lbrace d_{ij}^{(k-1)}, d_{ik}^{(k-1)} + d_{kj}^{(k-1)} \rbrace  & \text{if } k \geq 1
\end{cases}
$$

After $n-1$ iterations, we have $d_{ij}^{(n-1)} = \delta_{ij}$, which is the weight of the shortest path between any two vertices $i$ and $j$ in the given graph.

Implementation: [APSP - Floyd-Warshall](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/floyd/apsp-3.c)

Notice that the code is very similar to the previous solution, except that we iterate over the intermediate vertices $k$ first, and then over the pairs of vertices $i$ and $j$. We also omit the outermost loop of $n-1$ or $\lceil \log_2 (n-1) \rceil$ iterations. Thus, the time complexity of the Floyd-Warshall algorithm is able to drop to $\Theta(n^3)$.

**Note**: try switching the order of the loops in the Floyd-Warshall algorithm, for example by iterating over the pairs of vertices $i$ and $j$ first, and then over the intermediate vertices $k$. You will see that this produces wrong results. It shows that, although the code is very similar to the previous solution, the order of the loops is important, and that the Floyd-Warshall algorithm is not a generalization of the matrix multiplication algorithm, but a different interpretation of the APSP problem that leads to a different algorithm with a different time complexity.
