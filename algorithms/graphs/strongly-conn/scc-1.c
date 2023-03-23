/* file: scc.c
* author: David De Potter
* description: computes the strongly connected 
*              components of a directed graph
* complexity: O(n+m)
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  int id, dTime, fTime, parent, nbrCount, nbrCap;
  int *neighbors;   // adjacency list
} node;              

typedef struct graph {
  int nNodes, nEdges;
  node **vertices;  // array of pointers to nodes
} graph;

typedef struct list {
  node *n;
  struct list *next;
} list;

//::::::::::::::::::::: memory management ::::::::::::::::::::::::://

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

void checkCap(node *n) {
  /* checks whether the adjacency list of n is full, and if so, 
     doubles its capacity */
  if (n->nbrCount == n->nbrCap) {
    n->nbrCap = (n->nbrCap == 0) ? 1 : 2*n->nbrCap;
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

void addEge(graph *G, int u, int v) {
  /* adds an edge (u,v) to the graph G */
  node *n = G->vertices[u]; 
  G->nEdges++;
  // add v's id to u's adjacency list
  checkCap(n);
  n->neighbors[n->nbrCount++] = v;
}

void buildGraphs(graph *G, graph *GT) {
  /* reads edges from stdin and adds them to the graph, 
     and builds the transposed graph at the same time*/
  int u, v;
  while (scanf("%d %d", &u, &v) == 2) {
    addEge(G, u, v);
    addEge(GT, v, u);
  }
}

//:::::::::::::::::::::::: list functions ::::::::::::::::::::::::://

list *newList() {
  /* creates an empty list */
  return NULL;
}

void printList(list *L) {
  /* prints the list L */
  printf("{"); 
  while (L != NULL) {
    printf("%d", L->n->id);
    if (L->next != NULL) printf(", ");
    L = L->next;
  }
  printf("}\n");
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

//:::::::::::::::::::::::: dfs functions :::::::::::::::::::::::::://

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

void dfsG(graph *G, list **L, int *time) {
  /* performs a depth-first search on the graph G */
  for (int i = 0; i < G->nNodes; i++) {
    node *n = G->vertices[i];
    if (n->dTime < 0)  // n is undiscovered
      dfsVisit(G, n, L, time);
  }
}

void dfsGT(graph *GT, list **L, int *time) {
  /* performs a depth-first search on the transposed graph GT */
  for (list *l = *L; l != NULL; l = l->next) {
    node *n = GT->vertices[l->n->id];
    if (n->dTime < 0) {  // n is undiscovered
      list *component = newList();
      dfsVisit(GT, n, &component, time);
      printList(component);
      freeList(component);
    }
  }
}

//::::::::::::::::::::::::: topSort ::::::::::::::::::::::::::::::://

list *topSort(graph *G) {
  /* performs a topological sort on the graph G */
  list *L = newList();
  int time = 0;
  dfsG(G, &L, &time);
  return L;
}

void decompose(graph *GT, list *L) {
  /* decomposes the graph GT into strongly connected components */
  int time = 0;
  printf("Strongly connected components:\n");
  dfsGT(GT, &L, &time);
}

void printEdges(graph *G) {
  /* prints the edges of the graph G */
  for (int i = 0; i < G->nNodes; i++) {
    node *n = G->vertices[i];
    for (int j = 0; j < n->nbrCount; j++)
      printf("%d %d\n", n->id, n->neighbors[j]);
  }
}

//::::::::::::::::::::::: main function ::::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int n;                    // n = number of nodes
  scanf("%d", &n); 

  graph *G = newGraph(n);   // original graph
  graph *GT = newGraph(n);  // transposed graph
  buildGraphs(G, GT);       // read edges from stdin

  list *L = topSort(G);     // topological sort
  decompose(GT, L);         // strongly connected components

  freeGraph(G);
  freeGraph(GT);
  freeList(L);
  return 0;
}