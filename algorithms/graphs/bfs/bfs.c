/* file: bfs.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: breadth-first search 
     with a queue implemented as a circular array and a graph
     implemented as an array of nodes with adjacency lists
   assumption: nodes are numbered 0..n-1
*/

#include <stdio.h>
#include <stdlib.h>

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

typedef struct node {
  int id, parent;         // node id and parent id
  int nbrCount, nbrCap;   // number of neighbors and adj list capacity
  int dist;               // distance from source
  int *neighbors;         // adjacency list: node ids of neighbors
} node;

typedef struct queue {
  int front, back, size;  // front and back of the queue, and its size
  int *array;             // array of elements in the queue
} queue;

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

//:::::::::::::::::::::::: queue functions ::::::::::::::::::::::::://

short isEmpty(queue *Q) {
  /* returns 1 if the queue is empty, 0 otherwise */
  return Q->front == Q->back;
}

queue *newQueue(int n) {
  /* creates a queue with n elements */
  queue *Q = safeCalloc(1, sizeof(queue));
  Q->array = safeCalloc(n, sizeof(int));
  Q->front = 0;
  Q->back = 0;
  Q->size = n;
  return Q;
}

void freeQueue(queue *Q) {
  /* frees all memory allocated for the queue */
  free(Q->array);
  free(Q);
}

void doubleQueueSize(queue *Q) {
  /* doubles the size of the queue */
  Q->array = safeRealloc(Q->array, 2 * Q->size * sizeof(int));
  for (int i = 0; i < Q->back; ++i){
    Q->array[i + Q->size] = Q->array[i];
  }
  Q->back += Q->size;
  Q->size *= 2;
}

void enqueue (queue *Q, int n) {
  /* adds n to the back of the queue */
  Q->array[Q->back] = n;
  Q->back = (Q->back + 1) % Q->size;
  if (Q->back == Q->front) doubleQueueSize(Q);
}

int dequeue (queue *Q) {
  /* removes and returns the first element of the queue */
  if (isEmpty(Q)) {
    printf("Error: dequeue() called on empty queue.\n");
    exit(EXIT_FAILURE);
  }
  int n = Q->array[Q->front];
  Q->front = (Q->front + 1) % Q->size;
  return n;
}

//:::::::::::::::::::::::: graph functions ::::::::::::::::::::::::://

node *newNode(int id) {
  /* creates a node with given id */
  node *n = safeCalloc(1, sizeof(node));
  n->id = id;
  n->dist = 0;
  n->parent = -1; // -1 means no parent
  n->nbrCount = 0;
  n->nbrCap = 0;
  return n;
}

graph *newGraph(int n) {
  /* creates a graph with n vertices */
  graph *G = safeCalloc(1, sizeof(graph));
  G->nNodes = n;
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

void checkCap(node *n) {
  /* checks whether the adjacency list of n is large enough */
  if (n->nbrCount == n->nbrCap) {
    n->nbrCap = (n->nbrCap == 0) ? 2 : 2 * n->nbrCap;
    n->neighbors = safeRealloc(n->neighbors, n->nbrCap * sizeof(int));
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

//::::::::::::::::::::::::: bfs functions :::::::::::::::::::::::::://

void printPath(graph *G, int s, int d) {
  /* recursively prints the path from s to d */
  if (d == s) printf("%d", s);
  else {
    printPath(G, s, G->vertices[d]->parent);
    printf(", %d", d);
  }
}

void printResult(graph *G, int s, int d) {
  /* prints the result of the bfs */
  if (G->vertices[d]->parent >= 0) {
    // if d's parent is set, there is a path from s to d
    printf("Distance from %d to %d: %d\n", s, d, G->vertices[d]->dist);
    printf("Path: [");
    printPath(G, s, d);
    printf("]\n");
  } else printf("There is no path from %d to %d.\n", s, d);
}

void bfs(graph *G, int s) {
  /* performs a breadth-first search on the graph G starting at node s */
  queue *q = newQueue(G->nNodes); 
  enqueue(q, s); // enqueue source node
  while (!isEmpty(q)) {
    node *n = G->vertices[dequeue(q)]; 
   
    for (int i = 0; i < n->nbrCount; i++) {   // check each neighbor
      node *a = G->vertices[n->neighbors[i]];
      if (a->parent < 0) {
        // set parent and update distance
        a->parent = n->id;
        a->dist = n->dist + 1;
        enqueue(q, a->id);
      }
    }
  }
  freeQueue(q);
}

//::::::::::::::::::::::::: main function :::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int n, source, dest;  // n = number of vertices
  scanf("%d", &n);  
  scanf("%d %d", &source, &dest);

  if (source == dest) {
    printf("Distance from %d to %d: 0\n", source, dest);
    return 0;
  }
  
  graph *G = newGraph(n); 
  buildGraph(G);  // build graph from edges

  bfs(G, source);
  printResult(G, source, dest);

  freeGraph(G);
  return 0;
}