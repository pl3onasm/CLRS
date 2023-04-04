/* file: dijkstra-1.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: Dijkstra's shortest paths algorithm
     using a binary min-heap
   assumption: nodes are numbered 0..n-1
*/

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#define LEFT(i)   (2*i + 1)
#define RIGHT(i)  (2*i + 2)
#define PARENT(i) ((i-1)/2)

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

typedef struct list list;    // forward declaration

typedef struct node {
  // graph-related fields
  int id, parent;            // node id and parent id
  double dist;               // distance from source
  list *adj;                 // adjacency list

  // heap-related fields
  int heapIndex;             // index in the heap
  short inHeap;              // 1 if node is in the heap, 0 otherwise
  double key;                // key used to sort the heap
} node;

struct list {                
  node *n;                   // pointer to a node in the graph
  list *next;                // pointer to next node in the list
  double w;                  // weight of incoming edge at node n
};                           

typedef struct heap {
  int nNodes;                // number of elements in the heap
  node **nodes;              // array of pointers to nodes
} heap;

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

void freeList(list *L) {
  /* frees all memory allocated for the list */
  if (L == NULL) return;
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
  n->parent = -1;       // -1 indicates no parent
  n->dist = DBL_MAX;    // initialize distance to infinity
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
  /* builds the graph G by reading weighted edges from stdin */
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

//::::::::::::::::::::::::: heap functions ::::::::::::::::::::::::://

heap *newHeap(graph *G) {
  /* creates a heap with n nodes */
  int n = G->nNodes;
  heap *H = safeCalloc(1, sizeof(heap));
  H->nNodes = n;
  H->nodes = safeCalloc(n, sizeof(node*));
  // make pointer copies of the graph nodes 
  // and initialize the heap-related fields
  for (int i = 0; i < n; i++){
    node *u = G->nodes[i];
    H->nodes[i] = u;
    u->heapIndex = i;
    u->inHeap = 1;
    u->key = u->dist;
  }
  return H;
}

void freeHeap (heap *H) {
  /* frees the heap */
  free(H->nodes);
  free(H);
}

void swap (heap *H, int i, int j) {
  /* swaps the nodes at indices i and j in the heap */
  node *tmp = H->nodes[i];
  H->nodes[i] = H->nodes[j];
  H->nodes[j] = tmp;
  // update the heapIndex fields
  H->nodes[i]->heapIndex = i;
  H->nodes[j]->heapIndex = j;
}

void minHeapify(heap *H, int i){
  /* restores the min heap property in a top-down manner */
  int min = i, l = LEFT(i), r = RIGHT(i);
  if (l < H->nNodes && H->nodes[l]->key < H->nodes[i]->key)
    min = l;
  if (r < H->nNodes && H->nodes[r]->key < H->nodes[min]->key)
    min = r;
  if (min != i) {
    swap(H, i, min); 
    minHeapify(H, min);
  }
}

void decreaseKey(heap *H, int i, double newKey){
  /* decreases the key of the node at index i to newKey */
  if (newKey > H->nodes[i]->key) {
    printf("Error: new key is greater than current key\n");
    exit(EXIT_FAILURE);
  }
  H->nodes[i]->key = newKey;
  while (i > 0 && H->nodes[PARENT(i)]->key > H->nodes[i]->key) {
    swap(H, i, PARENT(i));
    i = PARENT(i);
  }
}

node *extractMin(heap *H) {
  /* extracts the node with minimum key from the heap */
  node *min = H->nodes[0];
  H->nodes[0] = H->nodes[--H->nNodes];
  minHeapify(H, 0);
  min->inHeap = 0;
  return min;
}

void initMinHeap(heap *H){
  /* initializes the min heap */
  for (int i = H->nNodes/2 - 1; i >= 0; i--)
    minHeapify(H, i);
}

//::::::::::::::::::::::: dijkstra functions ::::::::::::::::::::::://

short relax(node *u, node *v, double w) {
  /* relaxes the edge (u,v) */
  if (v->dist > u->dist + w) {
    v->dist = u->dist + w;
    v->parent = u->id;
    return 1;
  }
  return 0;
}

void dijkstra(graph *G, int s) {
  /* computes the shortest paths from node s to all other nodes */
  G->nodes[s]->dist = 0;
  heap *H = newHeap(G);
  initMinHeap(H);

  while (H->nNodes > 0) {
    node *u = extractMin(H);

    for (list *a = u->adj; a != NULL; a = a->next){
      node *v = a->n;
      if (relax(u, v, a->w) && v->inHeap)
        decreaseKey(H, v->heapIndex, v->dist);
    }
  }
  freeHeap(H);
}

void print(graph *G, int s) {
  /* prints the results of the shortest paths computation */
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
  int n, s;                    // n = number of nodes
  scanf("%d %d", &n, &s);      // s = source node 

  graph *G = newGraph(n); 
  buildGraph(G);               // read edges from stdin

  dijkstra(G, s);              // compute shortest paths from node s
  print(G, s);                 // print results

  freeGraph(G);
  return 0;
}
