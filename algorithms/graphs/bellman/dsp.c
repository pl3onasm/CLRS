/* file: dsp.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: DAG shortest paths algorithm
   assumption: nodes are numbered 0..n-1
*/

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

typedef struct list list;    // forward declaration

typedef struct node {
  int id, parent;            // node id and parent id
  double dist;               // distance from source
  int dTime, fTime;          // discovery and finish times
  list *adj;                 // adjacency list
} node;

struct list {                
  node *n;                   // pointer to a node in the graph
  list *next;                // pointer to next node in the list
  double w;                  // weight of incoming edge to n
};                           // (w is only used in adjacency lists)

typedef struct graph {
  int nNodes, nEdges;        // number of nodes and edges 
  node **nodes;              // array of pointers to nodes
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
  if (!L->next) printf(", ");
  printList(L->next);
}

void freeList(list *L) {
  /* frees all memory allocated for the list */
  if (!L) return;
  freeList(L->next);
  free(L);
}

list *listInsert (list *L, node *n, double w) {
  /* inserts the node n at the beginning of the list L */
  list *new = safeCalloc(1, sizeof(list));
  new->n = n;
  new->next = L;
  new->w = w;
  return new;
}

//::::::::::::::::::::::::: graph functions :::::::::::::::::::::::://

node *newNode(int id) {
  /* creates a new node with id id */
  node *n = safeCalloc(1, sizeof(node));
  n->id = id;
  n->parent = -1;
  n->dTime = n->fTime = -1;
  n->dist = DBL_MAX;
  n->adj = newList();
  return n;
}

graph *newGraph(int nNodes) {
  /* creates a new graph with nNodes nodes */
  graph *G = safeCalloc(1, sizeof(graph));
  G->nNodes = nNodes;
  G->nodes = safeCalloc(nNodes, sizeof(node*));
  for (int i = 0; i < nNodes; i++) 
    G->nodes[i] = newNode(i);
  return G;
}

void buildGraph (graph *G) {
  int u, v; double w;
  while (scanf("%d %d %lf", &u, &v, &w) == 3) {
    node *n = G->nodes[u];
    n->adj = listInsert(n->adj, G->nodes[v], w);
    G->nEdges++;
  }
}

void freeGraph(graph *G) {
  /* frees the memory allocated to the graph G */
  for (int i = 0; i < G->nNodes; i++) {
    freeList(G->nodes[i]->adj);
    free(G->nodes[i]);
  }
  free(G->nodes);
  free(G);
}

//:::::::::::::::::::::::: topological sort :::::::::::::::::::::::://

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
  *L = listInsert(*L, u, 0);
}

void dfs(graph *G, list **L, int *time) {
  /* performs a depth-first search on the graph G */
  for (int i = 0; i < G->nNodes; i++) {
    node *n = G->nodes[i];
    if (n->dTime < 0)  // n is undiscovered
      dfsVisit(G, n, L, time);
  }
}

list *topSort(graph *G) {
  /* performs a topological sort on the graph G */
  list *L = newList();
  int time = 0;
  dfs(G, &L, &time);
  return L;
}

//::::::::::::::::::::::::: shortest paths ::::::::::::::::::::::::://

void relax(node *u, node *v, double w) {
  /* relaxes the edge (u, v) */
  if (v->dist > u->dist + w) {
    v->dist = u->dist + w;
    v->parent = u->id;
  }
}

void shortestPaths(graph *G, int s) {
  /* computes the shortest paths from node s to all other nodes in G */
  list *L = topSort(G);     // topological sort of G
  G->nodes[s]->dist = 0;    // initialize source node

  for (list *l = L; l != NULL; l = l->next) {
    node *u = l->n;
    for (list *a = u->adj; a != NULL; a = a->next) {
      node *v = a->n;       // v is a successor of u
      relax(u, v, a->w);    // relax the edge (u, v)
    }
  }
  freeList(L);
}

void print (graph *G, int s) {
  /* prints the distances and parents of the nodes */
  printf("Node   Distance   Parent\n"); 
  for (int i = 0; i < G->nNodes; i++) {
    node *n = G->nodes[i];
    if (n->id == s) printf("%4s", "src");
    else printf("%4d", n->id);
    if (n->dist == DBL_MAX)
      printf("%11s", "inf");
    else printf("%11.2lf", n->dist);
    if (n->parent == -1)
      printf("%9c\n", '-');
    else if (n->parent == s)
      printf("%9s\n", "src");
    else printf("%9d\n", n->parent);
  }
}

//::::::::::::::::::::::::::::: main ::::::::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int n, s;                   // n = number of nodes 
  scanf("%d %d", &n, &s);     // s = source node

  graph *G = newGraph(n); 
  buildGraph(G);              // read edges from stdin

  shortestPaths(G, s);        // compute shortest paths from s
  print(G,s);                 // print results

  freeGraph(G);
  return 0;
}
