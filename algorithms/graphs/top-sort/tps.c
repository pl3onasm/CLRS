/* file: tps.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: topological sort of a directed acyclic 
     graph using depth-first search
   complexity: O(V + E)
*/

#include <stdio.h>
#include <stdlib.h>

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

typedef struct node {
  int id, parent;       // node id and parent id
  int nbrCount, nbrCap; // number of neighbors and adj list capacity
  int dTime, fTime;     // discovery and finish times
  int *neighbors;       // adjacency list: node ids of neighbors
} node;              

typedef struct graph {
  int nNodes, nEdges;   // number of nodes and edges in the graph
  node **vertices;      // array of pointers to nodes
} graph;

typedef struct list {
  node *n;              // pointer to node
  struct list *next;    // pointer to next node in the list
} list;

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
  n->fTime = -1;
  n->dTime = -1; 
  n->parent = -1; // -1 means no parent
  n->neighbors = NULL;
  n->nbrCount = 0;
  n->nbrCap = 0;
  return n;
}

void checkCap(node *n) {
  /* checks whether the adjacency list of n is full, and if so, 
     doubles its capacity */
  if (n->nbrCount == n->nbrCap) {
    n->nbrCap = (n->nbrCap == 0) ? 2 : 2*n->nbrCap;
    n->neighbors = safeRealloc(n->neighbors, n->nbrCap*sizeof(int));
  }
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
    free(G->vertices[i]);
  }
  free(G->vertices);
  free(G);
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

//::::::::::::::::::::::::: list functions ::::::::::::::::::::::::://

list *newList() {
  /* creates an empty list */
  return NULL;
}

void printList(list *L) {
  /* prints the list L */
  if (L == NULL) return;
  printf("%d", L->n->id);
  if (L->next != NULL) printf(", ");
  printList(L->next);
}

void freeList(list *L) {
  /* frees all memory allocated for the list */
  if (L == NULL) return;
  freeList(L->next);
  free(L);
}

list *listInsert (list *L, node *n) {
  /* inserts the node n at the beginning of the list L */
  list *new = safeCalloc(1, sizeof(list));
  new->n = n;
  new->next = L;
  return new;
}

//::::::::::::::::::::::::: dfs functions :::::::::::::::::::::::::://

void dfsVisit(graph *G, node *u, list **L, int *time) {
  /* visits the node u and its descendants in the graph G */
  u->dTime = ++*time;
  for (int i = 0; i < u->nbrCount; i++) {
    node *v = G->vertices[u->neighbors[i]];
    if (v->dTime < 0) {   // v is undiscovered
      v->parent = u->id;
      dfsVisit(G, v, L, time);
    } 
  }
  u->fTime = ++*time;
  *L = listInsert(*L, u);
}

void dfs(graph *G, list **L, int *time) {
  /* performs a depth-first search on the graph G */
  for (int i = 0; i < G->nNodes; i++) {
    node *n = G->vertices[i];
    if (n->dTime < 0)  // n is undiscovered
      dfsVisit(G, n, L, time);
  }
}

//:::::::::::::::::::::::: topological sort :::::::::::::::::::::::://

list *topSort(graph *G) {
  /* performs a topological sort on the graph G */
  list *L = newList();
  int time = 0;
  dfs(G, &L, &time);
  return L;
}

void printTopSort(list *L) {
  /* prints the result of the topological sort */
  printf("Topological sort:\n[");
  printList(L);
  printf("]\n");
}

//::::::::::::::::::::::::: main function :::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int n;  // n = number of nodes
  scanf("%d", &n); 

  graph *G = newGraph(n); 
  buildGraph(G);  // read edges from stdin

  list *L = topSort(G); // topological sort
  printTopSort(L);       

  freeGraph(G);
  freeList(L);
  return 0;
}