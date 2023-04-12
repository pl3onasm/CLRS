/* file: maxflow-5.c  
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: implements the relabel-to-front
                maximum flow algorithm
   time complexity: O(V^3)
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

typedef struct edge {
  int from, to;           // ids of the endpoints of the edge (u->v)
  double cap;             // capacity of the edge
  double flow;            // flow on the edge
  bool reverse;           // true if the edge is a reverse edge in Gf
  struct edge *rev;       // pointer to edge in the reverse direction
} edge;

typedef struct node {
  int id;                 // id of the node
  int *adj;               // adj list is an array of edge indices
  int adjCap;             // capacity of the adjacency list
  int nAdj;               // number of adjacent nodes
  int height;             // height of the node in the residual graph
  int current;            // current adjacency list index 
  double excess;          // excess flow at the node
  struct node *next;      // pointer to the next node in the worklist
  struct node *prev;      // pointer to the previous node in the worklist
} node;

typedef struct graph {
  int nNodes, nEdges;     // number of nodes and edges in the graph
  node **nodes;           // array of pointers to nodes
  edge **edges;           // array of pointers to edges
  int edgeCap;            // capacity of the edge array
  double maxFlow;         // maximum flow in the graph
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

//:::::::::::::::::::::::: graph functions ::::::::::::::::::::::::://

node *newNode(int id) {
  /* creates a node with given id */
  node *n = safeCalloc(1, sizeof(node));
  n->id = id;
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
  u->adj[u->nAdj++] = G->nEdges;  // add the edge index to the adj list
  G->edges[G->nEdges++] = e;      // add the edge to the edge array
  return e;
}

void buildGraph(graph *G) {
  /* reads undirected graph from stdin and builds the adjacency lists */
  int u, v; double cap;
  while (scanf("%d %d %lf", &u, &v, &cap) == 3) {
    addEdge(G, u, v, cap, false); // add original edge
    addEdge(G, v, u, 0, true);    // add reverse edge
    // add pointers to the reverse edges
    G->edges[G->nEdges-2]->rev = G->edges[G->nEdges-1];
    G->edges[G->nEdges-1]->rev = G->edges[G->nEdges-2];
  }
}

//::::::::::::::::::::::: push and relabel ::::::::::::::::::::::::://

node *newList(graph *G, int s, int t) {
  /* makes a doubly linked worklist of all nodes in G except for  
     s and t, by setting the next and prev pointers of each node */
  node *L = NULL;
  for (int i = G->nNodes-1; i >= 0; i--) 
    if (i != s && i != t) {
      G->nodes[i]->next = L;
      L = G->nodes[i];
      if (L->next != NULL)
        L->next->prev = L;
    }
  return L;
}

void initPreflow(graph *G, int s) {
  /* initializes the preflow at the source s */
  node *u = G->nodes[s];
  u->height = G->nNodes;      // set height of source to n
  for (int i = 0; i < u->nAdj; i++) {
    // set full flow on all edges from s
    int eId = u->adj[i];      // get edge index from adj list
    edge *e = G->edges[eId];
    e->flow = e->cap;         // set flow on original edge
    e->rev->flow = -e->cap;   // set flow on reverse edge
    G->nodes[e->to]->excess += e->cap;  // update excess at v
  }
}

void push(graph *G, node *u, node *v, edge *e) {
  /* pushes flow from u to v along edge e */
  double delta = MIN(u->excess, e->cap - e->flow);
  e->flow += delta;           // update flow on original edge
  e->rev->flow -= delta;      // update flow on reverse edge
  u->excess -= delta;         // update excess at u
  v->excess += delta;         // update excess at v
}

void relabel(graph *G, node *u) {
  /* relabels u to the minimum height of its 
     neighbors in Gf plus one */
  int min = INF;
  for (int i = 0; i < u->nAdj; i++) {
    edge *e = G->edges[u->adj[i]];
    node *v = G->nodes[e->to];
    if (e->cap - e->flow > 0 && v->height < min) 
      min = MIN(min, v->height);
  }
  u->height = min + 1;
}

void discharge(graph *G, node *u) {
  /* discharges the excess at u, i.e. pushes all excess flow 
     from u to its neighbors in Gf, relabeling u if needed */
  while (u->excess > 0) {
    if (u->current < u->nAdj){ // if there are still neighbors to push to
      edge *e = G->edges[u->adj[u->current]];
      node *v = G->nodes[e->to];
      if (e->cap - e->flow > 0 && u->height == v->height + 1) 
        push(G, u, v, e);
      else u->current++;       // move to next neighbor
    } else {                   // if no neighbors can be pushed to
      relabel(G, u);           // relabel u
      u->current = 0;          // reset current adj list index 
    }
  }
}

node *moveToFront(node *L, node *u) {
  /* moves node u to the front of worklist L */
  if (u->next) u->next->prev = u->prev;
  if (u->prev) u->prev->next = u->next;
  else L = u->next;            // if u is the head of the worklist
  u->prev = NULL;
  u->next = L;
  L->prev = u;
  return u;
}

void maxFlow (graph *G, int s, int t) {
  /* computes the maximum flow from s to t */
  initPreflow(G, s);                  // initialize the preflow
  node *listHead = newList(G, s, t);  // make a worklist  
  node *u = listHead;

  while (u) {
    int oldHeight = u->height;
    discharge(G, u);                  // discharge the excess at u
    if (u->height > oldHeight)        // if u was relabeled while discharging
      listHead = moveToFront(listHead, u);  
    u = u->next;
  }
  G->maxFlow = G->nodes[t]->excess;   // max flow is excess at t
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
  int n, s, t;                       // number of nodes, source, sink
  scanf("%d %d %d", &n, &s, &t);

  graph *G = newGraph(n); 
  buildGraph(G);                     // read edges from stdin

  maxFlow(G, s, t);                  // compute max flow
  printFlow(G, s, t);                // print flow values

  freeGraph(G);                      // free memory
  return 0;
}
