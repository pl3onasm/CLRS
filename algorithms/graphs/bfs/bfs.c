/* file: bfs.c
* author: David De Potter
* description: breadth-first search 
*   with a queue implemented as a circular array
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  int id, dist, parent;
  struct node *next;
} node;

typedef struct queue {
  int *array;
  int front, back, size; 
} queue;

typedef struct graph {
  int n;
  node **adj; 
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

//:::::::::::::::::::::::: queue functions ::::::::::::::::::::::::://

short isEmpty(queue *Q) {
  return Q->front == Q->back;
}

queue *newQueue(int n) {
  queue *Q = safeCalloc(1, sizeof(queue));
  Q->array = safeCalloc(n, sizeof(int));
  Q->front = 0;
  Q->back = 0;
  Q->size = n;
  return Q;
}

void freeQueue(queue *q) {
  free(q->array);
  free(q);
}

void doubleQueueSize(queue *Q) {
  Q->array = safeRealloc(Q->array, 2 * Q->size * sizeof(int));
  for (int i = 0; i < Q->back; ++i){
    Q->array[i + Q->size] = Q->array[i];
  }
  Q->back += Q->size;
  Q->size *= 2;
}

void enqueue (queue *Q, int n) {
  Q->array[Q->back] = n;
  Q->back = (Q->back + 1) % Q->size;
  if (Q->back == Q->front) doubleQueueSize(Q);
}

int dequeue (queue *Q) {
  if (isEmpty(Q)) {
    printf("Error: dequeue() called on empty queue.\n");
    exit(EXIT_FAILURE);
  }
  int n = Q->array[Q->front];
  Q->front = (Q->front + 1) % Q->size;
  return n;
}

//:::::::::::::::::::::::: graph functions ::::::::::::::::::::::::://

node *newNode(int v) {
  node *n = safeCalloc(1, sizeof(node));
  n->next = NULL;
  n->id = v;
  n->dist = 0;
  n->parent = -1;
  return n;
}

graph *newGraph(int n) {
  graph *G = safeCalloc(1, sizeof(graph));
  G->n = n;
  G->adj = safeCalloc(n, sizeof(node*));
  for (int i = 0; i < n; i++)
    G->adj[i] = newNode(i);
  return G;
}

void freeGraph(graph *G) {
  for (int i = 0; i < G->n; i++) {
    node *n = G->adj[i];
    while (n != NULL) {
      node *m = n;
      n = n->next;
      free(m);
    }
  }
  free(G->adj);
  free(G);
}

void addEdges(graph *G) {
  int u, v;
  while (scanf("%d %d", &u, &v) == 2) {
    node *n = newNode(v);
    n->next = G->adj[u]->next;
    G->adj[u]->next = n;
  }
}

//:::::::::::::::::::::::: bfs functions :::::::::::::::::::::::::://

void printPath(graph *G, int s, int d) {
  if (d == s) {
    printf("%d", s);
  } else {
    printPath(G, s, G->adj[d]->parent);
    printf(", %d", d);
  }
}

void printResult(graph *G, int s, int d) {
  if (G->adj[d]->parent >= 0) { 
    // if d's parent is set, there is a path from s to d
    printf("Distance from %d to %d: %d\n", s, d, G->adj[d]->dist);
    printf("Path: [");
    printPath(G, s, d);
    printf("]\n");
  } else printf("There is no path from %d to %d.\n", s, d);
}

void bfs(graph *G, int s) {
  queue *q = newQueue(G->n);
  enqueue(q, s); // enqueue source node
  while (!isEmpty(q)) {
    node *n = G->adj[dequeue(q)]; 
    node *m = n->next; 
   
    while (m) {
      node *a = G->adj[m->id];
      if (a->parent < 0) {
        // set parent and update distance
        a->parent = n->id;
        a->dist = n->dist + 1;
        enqueue(q, m->id);  
      }
      m = m->next;  // move to next adjacent node
    }
  }
  freeQueue(q);
}

//::::::::::::::::::::::: main function ::::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int n, source, dest;  // n = number of vertices
  scanf("%d", &n);  
  scanf("%d %d", &source, &dest);

  if (source == dest) {
    printf("Distance from %d to %d: 0\n", source, dest);
    return 0;
  }
  
  graph *G = newGraph(n); 
  addEdges(G);  // build graph from edges

  bfs(G, source);
  printResult(G, source, dest);

  freeGraph(G);
  return 0;
}
