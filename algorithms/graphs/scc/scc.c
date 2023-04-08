/* file: scc.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: computes the strongly connected 
     components of a directed graph
   complexity: O(n+m)
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

typedef struct graph {
  int nNodes, nEdges;      // number of nodes and edges in the graph
  node **vertices;         // array of pointers to nodes
} graph;

struct list {
  node *n;                 // pointer to node
  struct list *next;       // pointer to next node in the list
};

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

//:::::::::::::::::::::::: list functions :::::::::::::::::::::::::://

list *newList() {
  /* creates an empty list */
  return NULL;
}

void printList(list *L) {
  /* prints the list L */
  printf("{"); 
  while (L) {
    printf("%d", L->n->id);
    if (L->next != NULL) printf(", ");
    L = L->next;
  }
  printf("}\n");
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

void buildGraphs(graph *G, graph *GT) {
  /* reads edges from stdin and adds them to the graph G,
     and builds the transposed graph Gᵀ at the same time*/
  int u, v;
  while (scanf("%d %d", &u, &v) == 2) {
    node *n = G->vertices[u];
    n->adj = listInsert(n->adj, G->vertices[v]);
    n = GT->vertices[v];
    n->adj = listInsert(n->adj, GT->vertices[u]);
    G->nEdges++;
  }
}

//::::::::::::::::::::::::: dfs functions :::::::::::::::::::::::::://

void dfsVisit(graph *G, node *u, list **L, int *time) {
  /* visits the node u and its descendants in the graph G */
  u->dTime = ++*time;
  for (list *l = u->adj; l; l = l->next) {
    node *v = l->n;
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
    if (n->dTime < 0)    // n is undiscovered in G
      dfsVisit(G, n, L, time);
  }
}

void dfsGT(graph *GT, list *L, int *time) {
  /* performs a depth-first search on the transposed graph Gᵀ and
     prints the strongly connected components of G on the fly */
  for (list *l = L; l; l = l->next) {
    node *n = GT->vertices[l->n->id];
    if (n->dTime < 0) {  // n is undiscovered in Gᵀ
      list *component = newList();
      dfsVisit(GT, n, &component, time); 
      printList(component);
      freeList(component);
    }
  }
}

//::::::::::::::: topological sort and decomposition ::::::::::::::://

list *topSort(graph *G) {
  /* performs a topological sort on the graph G */
  list *L = newList();
  int time = 0;
  dfsG(G, &L, &time);
  return L;
}

void decompose(graph *GT, list *L) {
  /* decomposes the graph into strongly connected components 
     using a topological sort on Gᵀ and a list L of nodes in
     decreasing order of finishing time in G */
  printf("Strongly connected components:\n");
  int time = 0;
  dfsGT(GT, L, &time);
}

//:::::::::::::::::::::::: main function ::::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int n;                    // n = number of nodes
  scanf("%d", &n); 

  graph *G = newGraph(n);   // original graph G
  graph *GT = newGraph(n);  // transposed graph Gᵀ
  buildGraphs(G, GT);       // read edges from stdin

  list *L = topSort(G);     // topological sort
  decompose(GT, L);         // get the strongly connected components

  freeGraph(G);
  freeGraph(GT);
  freeList(L);
  return 0;
}