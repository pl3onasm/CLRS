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

typedef struct list list;  // forward declaration

typedef struct node {
  int id, parent;          // node id and parent id
  int dTime, fTime;        // discovery and finish times
  list *adj;               // adjacency list
  char color;              // white, gray, or black
} node;              

struct list {
  node *nbr;               // pointer to neighbor node
  list *next;              // pointer to next node in the list
  char type;               // edge type (tree, back, forward, cross)
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
  printf("%d", L->nbr->id);
  if (L->next != NULL) printf(", ");
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
  new->nbr = n;
  new->next = L;
  return new;
}

//:::::::::::::::::::::::: graph functions ::::::::::::::::::::::::://

node *newNode(int id) {
  /* creates a node with given id */
  node *n = safeCalloc(1, sizeof(node));
  n->id = id;
  n->dTime = -1; 
  n->parent = -1; // -1 means no parent
  n->color = 'w';
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
  /* reads edges from stdin and adds them to the directed graph */
  int u, v;
  while (scanf("%d %d", &u, &v) == 2) {
    node *n = G->vertices[u];
    G->nEdges++;
    n->adj = listInsert(n->adj, G->vertices[v]);
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
    printf("%7d %11d %9d\n", n->id, n->dTime, n->fTime);
  }
  printf("\nEdge classification\n\n  Starting   Ending      Edge\n"
         "      node     node      type\n");
  for (int i = 0; i < G->nNodes; i++) {
    node *n = G->vertices[i];
    for (list *a = n->adj; a != NULL; a = a->next) {
      printf("%10d %8d %9c\n", n->id, a->nbr->id, a->type);
    }
  }
}

void dfsVisit(graph *G, node *u, int *time) {
  /* visits the node u and its descendants in the graph G */
  u->dTime = ++*time;
  u->color = 'g';
  
  for (list *L = u->adj; L; L = L->next) {
    node *v = L->nbr;
    if (v->color == 'w') {
      L->type = 'T';     // tree edge
      v->parent = u->id;
      dfsVisit(G, v, time);
    } 
    else if (v->color == 'g') 
      L->type = 'B';     // back edge
    else if (v->dTime > u->dTime)  
      L->type = 'F';     // forward edge
    else if (v->dTime < u->dTime) 
      L->type = 'C';     // cross edge
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