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
#define INF DBL_MAX
#define true 1
#define false 0
#define bool short

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

typedef struct edge {
  int from, to;           // ids of the endpoints of the edge (u->v)
  double cap;             // capacity of the edge
  double flow;            // flow on the edge
  bool reverse;           // true if the edge is a reverse edge
} edge;

typedef struct node {
  int id;                 // id of the node
  int *adj;               // adjacency list: indices of the outgoing edges
  int adjCap;             // capacity of the adjacency list
  int nAdj;               // number of adjacent nodes
} node;

typedef struct graph {
  int nNodes, nEdges;     // number of nodes and edges in the graph
  node **nodes;           // array of pointers to nodes
  edge **edges;           // array of pointers to edges
  int edgeCap;            // capacity of the edge array
  double maxFlow;         // maximum flow in the graph
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
  G->maxFlow = 0;
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

edge *addEdge(graph *G, int uId, int vId, double cap, bool reverse) {
  /* adds an edge from u to v with capacity cap */
  edge *e = safeCalloc(1, sizeof(edge));
  e->from = uId;
  e->to = vId;
  e->cap = cap;
  e->flow = 0;
  e->reverse = reverse;
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
    addEdge(G, u, v, cap, false); // add original edge
    addEdge(G, v, u, 0, true);    // add reverse edge
  }
}

//:::::::::::::::::::::::: queue functions ::::::::::::::::::::::::://

bool isEmpty(queue *Q) {
  /* is true if the queue is empty */
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
  for (int i = 0; i < Q->back; ++i)
    Q->array[i + Q->size] = Q->array[i];
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
  double flow = INF;
  queue *q = newQueue(G->nNodes); 
  enqueue(q, s);    // enqueue source node
  while (!isEmpty(q)) {
    node *n = G->nodes[dequeue(q)]; 
   
    // visit all outgoing edges from n
    for (int i = 0; i < n->nAdj; i++) {  
      int eId = n->adj[i];
      edge *e = G->edges[eId];
      int a = e->to;
      if (e->cap - e->flow > 0 && path[a] == -1) {
        flow = MIN(flow, e->cap - e->flow);
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

void edmondsKarp(graph *G, int s, int t) {
  /* finds the maximum flow from s to t using Edmonds-Karp */
  int *path = safeCalloc(G->nNodes, sizeof(int));  
  double flow;
  while ((flow = bfs(G, s, t, path))) {
    G->maxFlow += flow;
    // update flow on each edge in the path
    for (int i = t; i != s; i = G->edges[path[i]]->from){
      edge *e = G->edges[path[i]];
      edge *r = G->edges[path[i]^1];
      e->flow += flow;   // update flow on original edge
      r->flow -= flow;   // update flow on reverse edge
    }
  }
  free(path);
}

void printFlow(graph *G, int s, int t) {
  /* prints the flow on each edge of the graph G */
  printf("The maximum flow from node %d to node %d"
         " is %.2lf\nFlow graph:\n\n  from     to%13s\n\n",
          s, t, G->maxFlow, "flow");
  for (int i = 0; i < G->nEdges; ++i) {
    edge *e = G->edges[i];
    if (!e->reverse){
      printf("%6d %6d", e->from, e->to);
      if (e->flow > 0) printf("%13.2lf\n", e->flow);
      else printf("%13c\n", '-');
    }
  }
}

//::::::::::::::::::::::::: main function :::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int n, s, t;                    // number of nodes, source, sink
  scanf("%d %d %d", &n, &s, &t);

  graph *G = newGraph(n); 
  buildGraph(G);                  // read edges from stdin

  edmondsKarp(G, s, t);           // find the maximum flow
  printFlow(G, s, t);             // print flow values

  freeGraph(G);                   // free memory
  return 0;
}