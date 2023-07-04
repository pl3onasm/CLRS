/* file: tps.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: topological sort of a directed acyclic 
     graph using depth-first search
   complexity: Θ(V + E)
*/

#include <stdio.h>
#include <stdlib.h>

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

typedef struct list list;  // forward declaration

typedef struct node {
  int id, parent;          // node id and parent id
  int dTime, fTime;        // discovery and finish times
  list *adj;               // adjacency list
} node;              

struct list {
  node *n;                 // pointer to node in the graph
  list *next;              // pointer to next node in the list
};

typedef struct graph {
  int nNodes, nEdges;      // number of nodes and edges in the graph
  node **vertices;         // array of pointers to nodes
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

//::::::::::::::::::::::::: list functions ::::::::::::::::::::::::://

list *newList() {
  /* creates an empty list */
  return NULL;
}

void printList(list *L) {
  /* prints the list L */
  if (!L) return;
  printf("%d", L->n->id);
  if (L->next) printf(", ");
  printList(L->next);
}

void freeList(list *L) {
  /* frees all memory allocated for the list */
  if (!L) return;
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

//:::::::::::::::::::::::: graph functions ::::::::::::::::::::::::://

node *newNode(int id) {
  /* creates a node with given id */
  node *n = safeCalloc(1, sizeof(node));
  n->id = id;
  n->fTime = -1;
  n->dTime = -1; 
  n->parent = -1; // -1 means no parent
  n->adj = newList();
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
    freeList(G->vertices[i]->adj);
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
    // add v to u's adjacency list
    n->adj = listInsert(n->adj, G->vertices[v]);
  }
}

//::::::::::::::::::::::::: dfs functions :::::::::::::::::::::::::://

void dfsVisit(graph *G, node *u, list **L, int *time) {
  /* visits the node u and its descendants in the graph G */
  u->dTime = ++*time;
  for (list *l = u->adj; l; l = l->next) {
    node *v = l->n;
    if (v->dTime < 0) {  // v is undiscovered
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
    if (n->dTime < 0)   // n is undiscovered
      dfsVisit(G, n, L, time);
  }
}

//:::::::::::::::::::::::: topological sort :::::::::::::::::::::::://

list *topSort(graph *G) {
  /* performs a topological sort on the graph G */
  list *L = newList();  // list to store the nodes in topological order
  int time = 0;         // time counter
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