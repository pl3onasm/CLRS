/* file: dfs.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: depth-first search,
     with a queue implemented as a circular array and a graph
     implemented as an array of nodes with adjacency lists
   assumption: nodes are numbered 0..n-1
   input: directed graph 
   output: discovery and finish times for each node and
           the edge types (tree, back, forward, cross)
*/

#include <stdio.h>
#include <stdlib.h>

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://
typedef struct node {
  int id, parent;         // node id and parent id
  int nbrCount, nbrCap;   // number of neighbors and adj list capacity
  int dTime, fTime;       // discovery and finish times
  int *neighbors;         // adjacency list: node ids of neighbors
  char *type;             // edge type
  char color;             // white, gray, or black
} node;              

typedef struct graph {
  int nNodes, nEdges;     // number of nodes and edges in the graph
  node **vertices;        // array of pointers to nodes
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
  n->dTime = -1; 
  n->parent = -1; // -1 means no parent
  n->nbrCount = 0;
  n->nbrCap = 0;
  n->color = 'w';
  return n;
}

graph *newGraph(int n) {
  /* creates a graph with n vertices */
  graph *G = safeCalloc(1, sizeof(graph));
  G->nNodes = n;
  G->nEdges = 0;
  G->vertices = safeCalloc(n, sizeof(node*));
  for (int i = 0; i < n; i++)
    G->vertices[i] = newNode(i);
  return G;
}

void freeGraph(graph *G) {
  /* frees all memory allocated for the graph */
  for (int i = 0; i < G->nNodes; i++) {
    free(G->vertices[i]->neighbors);
    free(G->vertices[i]->type);
    free(G->vertices[i]);
  }
  free(G->vertices);
  free(G);
}

void checkCap(node *n) {
  /* checks whether the adjacency list of n is large enough */
  if (n->nbrCount == n->nbrCap) {
    n->nbrCap = (n->nbrCap == 0) ? 2 : 2 * n->nbrCap;
    n->neighbors = safeRealloc(n->neighbors, n->nbrCap * sizeof(int));
    n->type = safeRealloc(n->type, n->nbrCap * sizeof(char));
  }
}

void buildGraph(graph *G) {
  /* reads edges from stdin and adds them to the graph */
  int u, v;
  while (scanf("%d %d", &u, &v) == 2) {
    node *n = G->vertices[u];
    G->nEdges++;
    checkCap(n);
    // add v's id to u's adjacency list
    n->neighbors[n->nbrCount++] = v;
  }
}

//::::::::::::::::::::::::: dfs functions :::::::::::::::::::::::::://

void printResults(graph *G) {
  /* prints the discovery and finish times of the nodes in the graph G 
     and the type of each edge */
  printf("Discovery and finish times\n\n   Node   Discovery    Finish\n"
         "  label        time      time\n");
  for (int i = 0; i < G->nNodes; i++) {
    node *n = G->vertices[i];
    printf("%7d    %8d    %6d\n", n->id, n->dTime, n->fTime);
  }
  printf("\nEdge classification\n\n  Starting   Ending      Edge\n"
         "      node     node      type\n");
  for (int i = 0; i < G->nNodes; i++) {
    node *n = G->vertices[i];
    for (int j = 0; j < n->nbrCount; j++) {
      node *a = G->vertices[n->neighbors[j]];
      printf("  %8d   %6d      %4c\n", n->id, a->id, n->type[j]);
    }
  }
}

void dfsVisit(graph *G, node *u, int *time) {
  /* visits the node u and its descendants in the graph G */
  u->dTime = ++*time;
  u->color = 'g';
  
  for (int i = 0; i < u->nbrCount; i++) {
    node *v = G->vertices[u->neighbors[i]];
    if (v->color == 'w') {
      u->type[i] = 'T';   // tree edge
      v->parent = u->id;
      dfsVisit(G, v, time);
    } 
    else if (v->color == 'g') 
      u->type[i] = 'B';   // back edge
    else if (v->dTime > u->dTime)  
      u->type[i] = 'F';   // forward edge
    else if (v->dTime < u->dTime) 
      u->type[i] = 'C';   // cross edge
  }
  u->fTime = ++*time;
  u->color = 'b';
}

void dfs(graph *G, int *time) {
  /* performs a depth-first search on the graph G */
  for (int i = 0; i < G->nNodes; i++) {
    node *n = G->vertices[i];
    if (n->color == 'w')
      dfsVisit(G, n, time);
  }
}

//::::::::::::::::::::::::: main function :::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int n, time = 0;  // n = number of nodes
  scanf("%d", &n); 

  graph *G = newGraph(n); 
  buildGraph(G);  // read edges from stdin

  dfs(G, &time);  
  printResults(G);

  freeGraph(G);
  return 0;
}