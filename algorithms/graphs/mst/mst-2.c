/* file: mst-2.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: implements Prim's algorithm to compute the minimum
     spanning tree of a graph. For this, we use a min priority queue 
     to keep track of the edges with the smallest weight. In this
     version, we use a binary min heap to implement the priority 
     queue.
   complexity: O(m log n) where m is the number of edges 
     and n is the number of nodes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#define LEFT(i)   (2*i + 1)
#define RIGHT(i)  (2*i + 2)
#define PARENT(i) ((i-1)/2)

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

typedef struct node {
  // graph-related fields
  int id, parent;         // node id and parent id
  int nbrCount, nbrCap;   // number of neighbors and adj list capacity
  int mstNode;            // 1 if node is in the MST, 0 otherwise
  int *neighbors;         // adjacency list: node ids of neighbors
  double *weights;        // corresponding weights of edges to neighbors
  // heap-related fields
  double key;             // keeps track of the minimum weight
  int heapIndex;          // node index in the heap
} node;

typedef struct graph {
  int nNodes, nEdges;     // number of nodes and edges in the graph
  node **vertices;        // array of pointers to nodes
} graph;

typedef struct heap {     // binary min heap
  node **nodes;           // array of pointers to nodes
  int nNodes, nodeCap;    // number of nodes and capacity
} heap;

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
  n->parent = -1;   // -1 means no parent
  n->nbrCount = 0;
  n->nbrCap = 0;
  n->mstNode = 0;   // 0 means not in the MST yet
  n->key = DBL_MAX; // initialize key to infinity
  return n;
}

void checkCap(node *n) {
  /* checks whether the adjacency list of n is full, and if so, 
     doubles capacities for neighbors and weights */
  if (n->nbrCount == n->nbrCap) {
    n->nbrCap = (n->nbrCap == 0) ? 2 : 2*n->nbrCap;
    n->neighbors = safeRealloc(n->neighbors, n->nbrCap*sizeof(int));
    n->weights = safeRealloc(n->weights, n->nbrCap*sizeof(double));
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
    free(G->vertices[i]->weights);
    free(G->vertices[i]);
  }
  free(G->vertices);
  free(G);
}

void updateAdjList(node *u, int v, double w) {
  /* adds the edge (n,v) to the adjacency list of n */
  checkCap(u);
  u->neighbors[u->nbrCount] = v;
  u->weights[u->nbrCount++] = w;
}

void buildGraph(graph *G) {
  int u, v; double w;
  while (scanf("%d %d %lf", &u, &v, &w) == 3) {
    updateAdjList(G->vertices[u], v, w);
    updateAdjList(G->vertices[v], u, w);
    G->nEdges++;
  }
}

//::::::::::::::::::::::::: heap functions ::::::::::::::::::::::::://

heap *newHeap(graph *G) {
  /* creates a heap with n nodes */
  int n = G->nNodes;
  heap *hp = safeCalloc(1, sizeof(heap));
  hp->nNodes = n;
  hp->nodeCap = n;
  hp->nodes = safeCalloc(n, sizeof(node*));
  // make pointer copies of the graph nodes 
  // and initialize the heapIndex fields
  for (int i = 0; i < n; i++){
    hp->nodes[i] = G->vertices[i];
    hp->nodes[i]->heapIndex = i;
  }
  return hp;
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
  return min;
}

void initMinHeap(heap *H){
  /* initializes the min heap */
  for (int i = H->nNodes/2 - 1; i >= 0; i--)
    minHeapify(H, i);
}

//::::::::::::::::::::::::: mst functions :::::::::::::::::::::::::://

void mstPrim(graph *G) {
  /* computes a minimum spanning tree of G using Prim's algorithm */
  G->vertices[0]->key = 0;  // set the key of the root to 0
  heap *H = newHeap(G);
  initMinHeap(H);
  
  while (H->nNodes > 0) {
    node *u = extractMin(H);
    u->mstNode = 1;     // mark the extracted node as part of the MST
    for (int i = 0; i < u->nbrCount; i++) {   
      // iterate over u's neighbors and update their keys
      node *v = G->vertices[u->neighbors[i]];  
      if (!v->mstNode && u->weights[i] < v->key) {
        v->parent = u->id;   // set v's parent to u
        decreaseKey(H, v->heapIndex, u->weights[i]);
      }
    }
  }
  freeHeap(H);
}

void printMST(graph *G) {
  /* prints the edges of the MST and its total weight */
  double totalWeight = 0;
  printf("MST edges:\n");
  for (int i = 0; i < G->nNodes; i++) {
    node *n = G->vertices[i];
    if (n->parent != -1 && n->mstNode == 1) {
      // reconstruct the edge (u, v, w) as (v.parent, v.id, v.key)
      printf("(%d, %d, %.2lf)\n", n->parent, n->id, n->key);
      totalWeight += n->key;
    } 
  }
  printf("MST weight: %.2lf\n", totalWeight);
}

//::::::::::::::::::::::::::::: main ::::::::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int n;                    // n = number of nodes
  scanf("%d", &n); 

  graph *G = newGraph(n); 
  buildGraph(G);            // read edges from stdin

  mstPrim(G);               // compute MST
  printMST(G);              // print MST edges and weight

  freeGraph(G);
  return 0;
}