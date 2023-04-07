/* file: maxflow-1.c  
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: implements the Edmonds-Karp version of the 
     max-flow algorithm. This is Ford-Fulkerson with BFS.
   time complexity: O(V*E^2) 
*/

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

typedef struct edge {
  int from, to;           // ids of the endpoints of the edge (u->v)
  double cap;             // capacity of the edge
  double flow;            // flow on the edge
  short residual;         // 1 if the edge is a residual edge
} edge;

typedef struct node {
  // graph-related fields
  int id;                 // id of the node
  int *adj;               // array of adjacent nodes: edge indices
  int adjCap;             // capacity of the adjacency list
  int nAdj;               // number of adjacent nodes
} node;

typedef struct graph {
  int nNodes, nEdges;     // number of nodes and edges in the graph
  node **nodes;           // array of pointers to nodes
  edge **edges;           // array of pointers to edges
  int edgeCap;            // capacity of the edge array
} graph;

typedef struct queue {
  int front, back, size;   // front and back of the queue, and its size
  int *array;              // array of elements in the queue
} queue;

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
  n->adjCap = 0;
  n->nAdj = 0;
  return n;
}

graph *newGraph(int n) {
  /* creates a graph with n vertices */
  graph *G = safeCalloc(1, sizeof(graph));
  G->nNodes = n;
  G->nEdges = 0;
  G->edgeCap = 0;
  G->nodes = safeCalloc(n, sizeof(node*));
  for (int i = 0; i < n; i++)
    G->nodes[i] = newNode(i);
  return G;
}

void freeGraph(graph *G) {
  /* frees all memory allocated for the graph */
  for (int i = 0; i < G->nNodes; i++){
    free(G->nodes[i]->adj);
    free(G->nodes[i]);
  }
  free(G->nodes);
  for (int i = 0; i < G->nEdges; i++)
    free(G->edges[i]);
  free(G->edges);
  free(G);
}

edge *addEdge(graph *G, int uId, int vId, double cap, short residual) {
  /* adds an edge from u to v with capacity cap */
  edge *e = safeCalloc(1, sizeof(edge));
  e->from = uId;
  e->to = vId;
  e->cap = cap;
  e->flow = 0;
  e->residual = residual;
  // check if we need to resize the edge array
  if (G->edgeCap == G->nEdges) {
    G->edgeCap += 10;
    G->edges = safeRealloc(G->edges, G->edgeCap * sizeof(edge*));
  }
  // check if we need to resize the adjacency list
  node *u = G->nodes[uId];
  if (u->adjCap == u->nAdj) {
    u->adjCap += 10;
    u->adj = safeRealloc(u->adj, u->adjCap * sizeof(int));
  }
  u->adj[u->nAdj++] = G->nEdges; // add the edge index to the adj list
  G->edges[G->nEdges++] = e;     // add the edge to the edge array
  return e;
}

void buildGraph(graph *G) {
  /* reads undirected graph from stdin and builds the adjacency lists */
  int u, v; double cap;
  while (scanf("%d %d %lf", &u, &v, &cap) == 3) {
    addEdge(G, u, v, cap, 0);   // add forward edge
    addEdge(G, v, u, 0, 1);     // add residual edge
  }
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

//::::::::::::::::::::::::: Edmonds-Karp ::::::::::::::::::::::::::://

double bfs(graph *G, int s, int t, int *path) {
  /* tries to find an augmenting path from s to t using BFS */
  memset(path, -1, G->nNodes * sizeof(int));
  double flow = DBL_MAX; 
  queue *q = newQueue(G->nNodes); 
  enqueue(q, s);    // enqueue source node
  while (!isEmpty(q)) {
    node *n = G->nodes[dequeue(q)]; 
   
    // check each edge from n
    for (int i = 0; i < n->nAdj; i++) {
      int eId = n->adj[i];
      edge *e = G->edges[eId];
      int a = e->to;
      if (e->cap > 0 && path[a] == -1) {
        flow = MIN(flow, e->cap);
        path[a] = eId;
        if (a == t) {
          freeQueue(q);
          return flow;
        }
        enqueue(q, a);
      }
    }
  }
  freeQueue(q);
  return 0;
}

double edmondsKarp(graph *G, int s, int t) {
  /* finds the maximum flow from s to t using Edmonds-Karp */
  int *path = safeCalloc(G->nNodes, sizeof(int));  
  double maxFlow = 0, flow;
  while (flow = bfs(G, s, t, path)) {
    maxFlow += flow;
    // update flow on each edge in the path
    for (int i = t; i != s; i = G->edges[path[i]]->from){
      edge *e = G->edges[path[i]];
      edge *r = G->edges[path[i]^1];
      e->flow += flow; e->cap -= flow;  // update forward edge
      r->flow -= flow; r->cap += flow;  // update residual edge
    }
  }
  free(path);
  return maxFlow;
}

void printFlow(graph *G, int s, int t, double maxFlow) {
  /* prints the flow on each edge of the graph G */
  printf("The maximum flow from node %d to node %d"
         " is %.2lf\n\nEdges %15s\n", s, t, maxFlow, "flow");
  for (int i = 0; i < G->nEdges; ++i) {
    edge *e = G->edges[i];
    if (!e->residual){
      printf("(%d, %d)", e->from, e->to);
      if (e->flow > 0) printf("%15.2lf\n", e->flow);
      else printf("%15c\n", '-');
    }
  }
}

//::::::::::::::::::::::::: main function :::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int n, s, t;                    // number of nodes, source, sink
  scanf("%d %d %d", &n, &s, &t);

  graph *G = newGraph(n); 
  buildGraph(G);                  // read edges from stdin

  double maxFlow = edmondsKarp(G, s, t); 
  printFlow(G, s, t, maxFlow);    // print flow values

  freeGraph(G);                   // free memory
  return 0;
}
