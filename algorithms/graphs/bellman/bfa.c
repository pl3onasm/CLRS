/* file: bfa.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: Bellman-Ford algorithm for single-source shortest paths
   assumption: nodes are numbered 0..n-1
*/

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

typedef struct edge {
  int u, v;              // edge endpoints
  double w;              // edge weight
} edge;

typedef struct node {
  int id, parent;        // node id and predecessor
  double dist;           // distance to the source 
} node;

typedef struct graph {
  int nNodes, nEdges;    // number of nodes and edges
  edge **edges;          // array of pointers to edges
  node **nodes;          // array of pointers to nodes
  int edgeCap;           // capacity of the edges array
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

//::::::::::::::::::::::::: edge functions ::::::::::::::::::::::::://

void checkEdgeCap(graph *G) {
  /* checks whether the edge array is full 
     and reallocates memory if necessary */
  if (G->nEdges == G->edgeCap) {
    G->edgeCap = G->edgeCap == 0 ? 10 : 2 * G->edgeCap;
    G->edges = safeRealloc(G->edges, G->edgeCap * sizeof(edge*));
  }
}

edge *newEdge(int u, int v, double w) {
  /* creates a new edge */
  edge *e = safeCalloc(1, sizeof(edge));
  e->u = u;
  e->v = v;
  e->w = w;
  return e;
}

//:::::::::::::::::::::::: graph functions ::::::::::::::::::::::::://

node *newNode(int id) {
  /* creates a node with given id */
  node *n = safeCalloc(1, sizeof(node));
  n->id = id;
  n->parent = -1;       // -1 means no parent
  n->dist = DBL_MAX;    // initially, all nodes are unreachable
  return n;
}

graph *newGraph(int n) {
  /* creates a graph with n vertices */
  graph *G = safeCalloc(1, sizeof(graph));
  G->nNodes = n;
  G->nEdges = 0;
  G->edgeCap = 0;
  G->nodes = safeCalloc(n, sizeof(node*));
  for (int i = 0; i < n; i++)
    G->nodes[i] = newNode(i);
  return G;
}

void buildGraph (graph *G){
  /* adds an edge to the graph */
  int u, v; double w;
  while (scanf("%d %d %lf", &u, &v, &w) == 3) {
  checkEdgeCap(G);
  G->edges[G->nEdges++] = newEdge(u, v, w);
  }
}

void freeGraph(graph *G) {
  /* frees the memory allocated for the graph */
  for (int i = 0; i < G->nNodes; i++)
    free(G->nodes[i]);
  free(G->nodes);
  for (int i = 0; i < G->nEdges; i++)
    free(G->edges[i]);
  free(G->edges);
  free(G);
}

//:::::::::::::::::::::::::: bellman-ford :::::::::::::::::::::::::://

void print (graph *G, short cycle) {
  /* prints the distances and parents of the nodes */
  if (cycle) {
    printf("Negative cycle found.\n");
    return;
  }
  printf("Node   Distance   Parent\n"); 
  for (int i = 0; i < G->nNodes; i++) {
    node *n = G->nodes[i];
    printf("%4d %10.2lf %8d\n", n->id, n->dist, n->parent);
  }
}

void relax(graph *G, edge *e) {
  /* relaxes the edge e */
  node *u = G->nodes[e->u];
  node *v = G->nodes[e->v];
  if (v->dist > u->dist + e->w) {
    v->dist = u->dist + e->w;
    v->parent = u->id;
  }
}

short containsCycle(graph *G, edge *e) {
  /* checks whether the edge e belongs to a negative cycle */
  node *u = G->nodes[e->u];
  node *v = G->nodes[e->v];
  return v->dist > u->dist + e->w;
}

short bFord(graph *G, int s) {
  /* runs the Bellman-Ford algorithm on graph G starting from node s
      returns 1 if a negative cycle is found, 0 otherwise */
  G->nodes[s]->dist = 0;
  for (int i = 0; i < G->nNodes - 1; i++) 
    for (int j = 0; j < G->nEdges; j++) 
        relax(G, G->edges[j]);

  for (int i = 0; i < G->nEdges; i++) 
    if (containsCycle(G, G->edges[i]))
      return 1;

  return 0;
}

//::::::::::::::::::::::::::::: main ::::::::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int n, s;                   // n = number of nodes 
  scanf("%d %d", &n, &s);     // s = source node

  graph *G = newGraph(n); 
  buildGraph(G);              // read edges from stdin

  short cycle = bFord(G, s);  // run Bellman-Ford algorithm
  print(G, cycle);            // print results

  freeGraph(G);
  return 0;
}