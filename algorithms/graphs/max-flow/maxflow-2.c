/* file: maxflow-2.c  
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: implements Ford-Fulkerson with DFS and a
     heuristic called capacity scaling to find the maximum flow
   time complexity: O(log(maxCap)*E^2)
*/

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define INF DBL_MAX
#define true 1
#define false 0
#define bool short

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

typedef struct edge {
  int from, to;           // ids of the endpoints of the edge (u->v)
  double cap;             // capacity of the edge
  double flow;            // flow on the edge
  bool residual;          // true if the edge is a residual edge
} edge;

typedef struct node {
  int id;                 // id of the node
  int *adj;               // adjacency list: indices of the outgoing edges
  int adjCap;             // capacity of the adjacency list
  int nAdj;               // number of adjacent nodes
  bool visited;           // true if the node has been visited in the DFS
} node;

typedef struct graph {
  int nNodes, nEdges;     // number of nodes and edges in the graph
  node **nodes;           // array of pointers to nodes
  edge **edges;           // array of pointers to edges
  int edgeCap;            // capacity of the edge array
  double maxCap;          // maximum capacity among all edges
  double maxFlow;         // maximum flow in the graph
} graph;

//::::::::::::::::::::::: memory management :::::::::::::::::::::::://

void *safeCalloc (int n, int size) {
  /* allocates n elements of size size, initializing them to 0, and
     checks whether the allocation was successful */
  void *ptr = calloc(n, size);
  if (ptr == NULL) {
    printf("Error: calloc(%d, %d) failed. Out of memory?\n", n, size);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void *safeRealloc (void *ptr, int newSize) {
  /* reallocates memory and checks whether the allocation was successful */
  ptr = realloc(ptr, newSize);
  if (ptr == NULL) {
    printf("Error: realloc(%d) failed. Out of memory?\n", newSize);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

//:::::::::::::::::::::::: graph functions ::::::::::::::::::::::::://

node *newNode(int id) {
  /* creates a node with given id */
  node *n = safeCalloc(1, sizeof(node));
  n->id = id;
  n->adjCap = 0;
  n->nAdj = 0;
  n->visited = false;
  return n;
}

graph *newGraph(int n) {
  /* creates a graph with n vertices */
  graph *G = safeCalloc(1, sizeof(graph));
  G->nNodes = n;
  G->nEdges = 0;
  G->edgeCap = 0;
  G->maxCap = 0;
  G->maxFlow = 0;
  G->nodes = safeCalloc(n, sizeof(node*));
  for (int i = 0; i < n; i++)
    G->nodes[i] = newNode(i);
  return G;
}

void freeGraph(graph *G) {
  /* frees all memory allocated for the graph */
  for (int i = 0; i < G->nNodes; i++){
    free(G->nodes[i]->adj);
    free(G->nodes[i]);
  }
  free(G->nodes);
  for (int i = 0; i < G->nEdges; i++)
    free(G->edges[i]);
  free(G->edges);
  free(G);
}

edge *addEdge(graph *G, int uId, int vId, double cap, bool residual) {
  /* adds an edge from u to v with capacity cap */
  edge *e = safeCalloc(1, sizeof(edge));
  e->from = uId;
  e->to = vId;
  e->cap = cap;
  e->flow = 0;
  e->residual = residual;
  // check if we need to resize the edge array
  if (G->edgeCap == G->nEdges) {
    G->edgeCap += 10;
    G->edges = safeRealloc(G->edges, G->edgeCap * sizeof(edge*));
  }
  // check if we need to resize the adjacency list
  node *u = G->nodes[uId];
  if (u->adjCap == u->nAdj) {
    u->adjCap += 10;
    u->adj = safeRealloc(u->adj, u->adjCap * sizeof(int));
  }
  u->adj[u->nAdj++] = G->nEdges; // add the edge index to the adj list
  G->edges[G->nEdges++] = e;     // add the edge to the edge array
  return e;
}

void buildGraph(graph *G) {
  /* reads undirected graph from stdin and builds the adjacency lists */
  int u, v; double cap;
  while (scanf("%d %d %lf", &u, &v, &cap) == 3) {
    G->maxCap = MAX(G->maxCap, cap);
    addEdge(G, u, v, cap, false);  // add forward edge
    addEdge(G, v, u, 0, true);     // add residual edge
  }
}

//:::::::::::::::::: DFS with capacity scaling ::::::::::::::::::::://

int pow2(int n) {
  /* returns the largest power of 2 ≤ the max capacity */
  int p = 1;
  while (p < n) p <<= 1;
  return p >> 1;
}

double dfs(graph *G, int s, int t, double flow, int delta) {
  /* tries to find an augmenting path from s to t using DFS */
  if (s == t) return flow;               // reached the sink
  node *u = G->nodes[s];
  if (u->visited) return 0;                 // already visited
  double bneck;
  for (int i = 0; i < u->nAdj; ++i) {     
    int eId = u->adj[i];
    edge *e = G->edges[eId];
    if (e->cap > delta) {             
      u->visited = true;                    // mark as visited
      if ((bneck = dfs(G, e->to, t, MIN(flow, e->cap), delta))) {
        edge *r = G->edges[eId ^ 1]; 
        e->cap -= bneck; e->flow += bneck;  // update forward edge              
        r->cap += bneck; r->flow -= bneck;  // update residual edge             
        u->visited = false;                 // mark as unvisited
        return bneck;
      }               
    }
  }
  u->visited = false; 
  return 0;
}

void maxFlow(graph *G, int s, int t) {
  /* finds the maximum flow from s to t using capacity scaling */
  double flow; 
  int delta = pow2(G->maxCap);        // initial threshold
  for (; delta > 0; delta >>= 1) {     
    while ((flow = dfs(G, s, t, INF, delta)))   
      G->maxFlow += flow;             // update the max flow
  }
}

void printFlow(graph *G, int s, int t) {
  /* prints the flow on each edge in the graph G */
  printf("The maximum flow from node %d to node %d"
         " is %.2lf\n\nEdges %15s\n", s, t, G->maxFlow, "Flow");
  printf("---------------------\n"); 
  for (int i = 0; i < G->nEdges; ++i) {
    edge *e = G->edges[i];
    if (!e->residual){
      printf("(%d, %d)", e->from, e->to);
      if (e->flow > 0) printf("%15.2lf\n", e->flow);
      else printf("%15c\n", '-');
    }
  }
}

//::::::::::::::::::::::::: main function :::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int n, s, t;                    // number of nodes, source, sink
  scanf("%d %d %d", &n, &s, &t);

  graph *G = newGraph(n);         // read edges from stdin
  buildGraph(G);                  // and get max capacity
                                  
  maxFlow(G, s, t);               // find maximum flow
  printFlow(G, s, t);             // print flow values

  freeGraph(G);                   // free memory
  return 0;
}
