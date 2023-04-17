/* file: mcbm-1.c  
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: Maximum Cardinality Bipartite Matching (MCBM) 
                using Dinic's algorithm for maximum flow
   time complexity: O(E√V)
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define INF INT_MAX
#define true 1
#define false 0
typedef short bool;

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

typedef struct node node; // forward declaration of node

typedef struct edge {
  node *from, *to;        // pointers to the endnodes of the edge (u->v)
  int cap;                // capacity of the edge
  int flow;               // flow on the edge
  bool reverse;           // true if the edge is a reverse edge
  struct edge *rev;       // pointer to edge in the reverse direction
} edge;

struct node {
  int id;                 // id of the node
  edge **adj;             // adj list is an array of pointers to edges
  int adjCap;             // capacity of the adjacency list
  int nAdj;               // number of adjacent nodes
  int level;              // level of the node in the level graph
  int adjIdx;             // current adj list index in the DFS traversals
};

typedef struct graph {
  int nNodes, nEdges;     // number of nodes and edges in the graph
  node **nodes;           // array of pointers to nodes
  edge **edges;           // array of pointers to edges
  int edgeCap;            // capacity of the edge array
  int maxFlow;            // maximum flow in the graph
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
  n->level = -1;
  return n;
}

graph *newGraph(int n) {
  /* creates a graph with n vertices */
  graph *G = safeCalloc(1, sizeof(graph));
  G->nNodes = n;
  G->nodes = safeCalloc(n, sizeof(node*));
  for (int i = 0; i < n; i++)
    G->nodes[i] = newNode(i);
  return G;
}

void loopReset(graph *G) {
  /* resets all level fields and current adjacency list indices */
  for (int i = 0; i < G->nNodes; i++){
    G->nodes[i]->level = -1;
    G->nodes[i]->adjIdx = 0;
  }
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

edge *addEdge(graph *G, int uId, int vId, int cap, bool reverse) {
  /* adds an edge from u to v with capacity cap */
  edge *e = safeCalloc(1, sizeof(edge));
  node *u = G->nodes[uId];
  e->to = G->nodes[vId];
  e->from = u;
  e->cap = cap;
  e->reverse = reverse;
  // check if we need to resize the edge array
  if (G->edgeCap == G->nEdges) {
    G->edgeCap += 10;
    G->edges = safeRealloc(G->edges, G->edgeCap * sizeof(edge*));
  }
  // check if we need to resize the adjacency list
  if (u->adjCap == u->nAdj) {
    u->adjCap += 10; 
    u->adj = safeRealloc(u->adj, u->adjCap * sizeof(edge*));
  }
  u->adj[u->nAdj++] = e;       // add the edge to the adj list                 
  G->edges[G->nEdges++] = e;   // add the original edge to G
  return e;
}

void buildGraph(graph *G, int left, int right) {
  /* reads undirected graph from stdin and builds the adjacency lists */
  int u, v; edge *e, *r;
  int n = G->nNodes;
  while (scanf("%d %d", &u, &v) == 2) {
    e = addEdge(G, u, v, 1, false);   // add original edge
    r = addEdge(G, v, u, 0, true);    // add reverse edge
    // add pointers to the reverse edges
    e->rev = r; r->rev = e;
  }
  // add edges from super source to left nodes
  for (int i = 0; i < left; i++) {
    e = addEdge(G, n-2, i, 1, false);
    r = addEdge(G, i, n-2, 0, true);
    e->rev = r; r->rev = e;
  }
  // add edges from right nodes to super sink
  for (int i = left; i < left+right; i++) {
    e = addEdge(G, i, n-1, 1, false);
    r = addEdge(G, n-1, i, 0, true);
    e->rev = r; r->rev = e;
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

//::::::::::::::::::::::::::::: Dinic :::::::::::::::::::::::::::::://

bool bfs(graph *G, int s, int t) {
  /* builds a BFS tree from s to t and returns if there is a path */
  queue *q = newQueue(G->nNodes); 
  enqueue(q, s);                           // enqueue source node
  G->nodes[s]->level = 0;                  // set source level to 0
  while (!isEmpty(q)) {
    node *n = G->nodes[dequeue(q)];
   
    // check each edge from n
    for (int i = 0; i < n->nAdj; i++) {
      edge *e = n->adj[i];
      if (e->cap - e->flow > 0 && e->to->level == -1) {
        e->to->level = n->level + 1;       // set level of child node
        enqueue(q, e->to->id);                
      }
    }
  }
  freeQueue(q);
  return G->nodes[t]->level != -1;         // return if t is reachable
}

int dfs(graph *G, int s, int t, int flow) {
  /* finds the blocking flow from s to t */
  if (s == t) return flow;                 // reached sink
  node *n = G->nodes[s];
  // check each edge from n and prune those that don't lead to t
  // so that we don't have to check them again in the next DFS call
  for (int i = n->adjIdx; i < n->nAdj; i++) {
    n->adjIdx = i;                         // update current adj list index
    edge *e = n->adj[i];
    if (e->cap - e->flow > 0 && e->to->level == n->level + 1) {
      int bneck = dfs(G, e->to->id, t, MIN(flow, e->cap - e->flow));
      if (bneck > 0) {
        e->flow += bneck;                  // adjust flow original edge
        e->rev->flow -= bneck;             // adjust flow reverse edge
        return bneck;
      } 
    } 
  }
  return 0;
}

void dinic(graph *G, int s, int t) {
  /* finds the maximum flow from s to t using Dinic's algorithm */
  while (bfs(G, s, t)) {
    for (int f = dfs(G, s, t, INF); f; f = dfs(G, s, t, INF)) 
      G->maxFlow += f;
    loopReset(G);  // reset all levels and current adjList indices
  }
}

void printFlow(graph *G, int s, int t) {
  /* prints the flow on each edge of the graph G */
  printf("Maximum matching has cardinality %d.\n", G->maxFlow);
  if (G->maxFlow == 0) return;
  printf("Possible matching:\n");
  for (int i = 0; i < G->nEdges; i++) {
    edge *e = G->edges[i];
    if (e->from->id != s && e->to->id != t 
        && e->flow > 0 && !e->reverse) 
      printf("%d → %d\n", e->from->id, e->to->id);
  }
}

//::::::::::::::::::::::::: main function :::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int l, r;                    // cardinality of left and right sets
  scanf("%d %d", &l, &r);
  int n = l + r + 2;           // number of nodes in the graph

  graph *G = newGraph(n);      // build an unweighted graph with
  buildGraph(G, l, r);         // super source and sink at the end

  dinic(G, n-2, n-1);          // find the maximum flow / matching
  printFlow(G, n-2, n-1);      // print the maximum matching

  freeGraph(G);                // free memory
  return 0;
}
