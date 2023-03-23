/* file: dfs.c
* author: David De Potter
* description: depth-first search 
*   with a queue implemented as a circular array and a graph
*   implemented as an array of nodes with adjacency lists
* assumption: nodes are numbered 0..n-1
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  int id, dTime, fTime, parent, nbrCount, nbrCap;
  int *neighbors;   // adjacency list
  char *type;       
} node;              

typedef struct graph {
  int nNodes, nEdges;
  node **vertices;  // array of pointers to nodes
} graph;

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
  // reallocates memory and checks whether the allocation was successful
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
  n->fTime = -1;
  n->dTime = -1; 
  n->parent = -1; // -1 means no parent
  n->neighbors = NULL;
  n->nbrCount = 0;
  n->nbrCap = 0;
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

void buildGraph(graph *G, short directed) {
  /* reads edges from stdin and adds them to the graph */
  int u, v;
  while (scanf("%d %d", &u, &v) == 2) {
    node *n = G->vertices[u];
    G->nEdges++;
    // add v's id to u's adjacency list
    if (n->nbrCount == n->nbrCap) {
      // if the adjacency list is full, double its size
      n->nbrCap = (n->nbrCap == 0) ? 2 : 2 * n->nbrCap;
      n->neighbors = safeRealloc(n->neighbors, n->nbrCap * sizeof(int));
      n->type = safeRealloc(n->type, n->nbrCap * sizeof(char));
    }
    n->neighbors[n->nbrCount++] = v;
  }
}

//:::::::::::::::::::::::: dfs functions :::::::::::::::::::::::::://

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

void classifyEdges(graph *G) {
  /* classifies the edges of the graph G */
  for (int i = 0; i < G->nNodes; i++) {
    node *u = G->vertices[i];
    printf("Node %d has parent %d\n", u->id, u->parent);
    for (int j = 0; j < u->nbrCount; j++) {
      node *v = G->vertices[u->neighbors[j]];
      if (v->fTime < u->fTime && v->parent == u->id)
        u->type[j] = 'T';  // tree edge
      else if (v->dTime < u->dTime && u->fTime < v->fTime)
        u->type[j] = 'B';  // back edge
      else if (v->dTime < u->dTime)
        u->type[j] = 'C';  // cross edge
      else if (u->dTime < v->dTime)
        u->type[j] = 'F';  // forward edge
    }
  }
}

void dfsVisit(graph *G, node *u, int *time) {
  /* visits the node u and its descendants in the graph G */
  u->dTime = ++*time;
  for (int i = 0; i < u->nbrCount; i++) {
    node *v = G->vertices[u->neighbors[i]];
    if (v->dTime < 0) {    // if v is not visited yet
      v->parent = u->id;
      dfsVisit(G, v, time);
    } 
  }
  u->fTime = ++*time;
}

void dfs(graph *G, int *time) {
  /* performs a depth-first search on the graph G */
  for (int i = 0; i < G->nNodes; i++) {
    node *n = G->vertices[i];
    if (n->dTime < 0)      // if n is not visited yet
      dfsVisit(G, n, time);
  }
}

//::::::::::::::::::::::: main function ::::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int n, time = 0;  // n = number of nodes
  short directed = 0;   // directed = 1 if graph is directed
  scanf("%d", &n);
  scanf("%hd", &directed);  

  graph *G = newGraph(n); 
  buildGraph(G, directed);    // build graph from edges

  dfs(G, &time);
  classifyEdges(G);
  printResults(G);

  freeGraph(G);
  return 0;
}