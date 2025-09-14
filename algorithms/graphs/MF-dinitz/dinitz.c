/* 
  file: ffm-3.c  
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: implements the Dinitz version of the 
    max-flow algorithm. This is Ford-Fulkerson with 
    level graphs and DFS.
  time complexity: O(V²E)
  note: make sure to use VERTEX_TYPE3 in the vertex.h file
    by defining it from the command line using
      $ gcc -D VERTEX_TYPE3 ...
*/

#include "../../../datastructures/graphs/network/network.h"
#include "../../../datastructures/queues/queue.h"
#include <assert.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

//===================================================================
// Breadth-first search to build the level graph
// Returns true if there is a path from source to sink
bool bfs(network *N, vertex *src, vertex *sink) {
  queue *q = newQueue(nVertices(N));       
  enqueue(q, src);                     // enqueue source node
  src->level = 1;     

  while (!isEmptyQueue(q)) {
    vertex *u = dequeue(q);
    dll *edges = getNeighbors(N, u);    
   
      // iterate over all outgoing edges of u
    for (edge *e = dllFirst(edges); e; e = dllNext(edges)) {
      if (e->cap - e->flow > 0 && !e->to->level) {
        e->to->level = u->level + 1;   // set level of child node
        enqueue(q, e->to);                
      }
    }
  }
  freeQueue(q);
  return sink->level != 0;             // is sink reachable?
}

//===================================================================
// Depth-first search to find an augmenting path in the level graph
// Returns the bottleneck flow found in the path
size_t dfs(network *N, vertex *v, vertex *sink, size_t flow) {
  
  if (v == sink || flow == 0) 
    return flow;                       // sink reached or no flow      
  
  dll *edges = getNeighbors(N, v);     

  for (edge *e = v->cont ? dllNext(edges) : dllFirst(edges); e; 
       e = dllNext(edges)) {

    v->cont = true;                    // set continue flag

    if (e->to->level == v->level + 1){ // child node in next level?

      size_t bneck = dfs(N, e->to, sink, 
                         MIN(flow, e->cap - e->flow));
      if (bneck) {
        e->flow += bneck;              // update flow
        e->rev->flow -= bneck;             
        return bneck;
      } 
    } 
  }
  return 0;
}

//===================================================================
// Computes the maximum flow from src to sink using Dinic's algorithm
void dinic(network *N, vertex *src, vertex *sink) {
  size_t flow;  

    // while there is a path from src to sink
  while (bfs(N, src, sink)) {
      // continue searching for augmenting paths
      // until a blocking flow is reached
    while ((flow = dfs(N, src, sink, SIZE_MAX))) 
      N->maxFlow += flow;                      
      
      // reset levels and continue flags for next iteration
    for (vertex *v = firstV(N); v; v = nextV(N)) {
      v->level = 0;
      v->cont = false;
    }
  }
}

//===================================================================

int main () {
    // read source and sink labels
  char srcL[50], sinkL[50];                    
  assert(scanf("%s %s ", srcL, sinkL) == 2);

  network *N = newNetwork(50, UNWEIGHTED);
  readNetwork(N);                  
  showNetwork(N);
  
  vertex *src = getVertex(N, srcL);     
  vertex *sink = getVertex(N, sinkL);     

  if (!src || !sink) {
    fprintf(stderr, "Source or sink not found\n");
    freeNetwork(N);
    exit(EXIT_FAILURE);
  }

  dinic(N, src, sink); 
  showFlow(N, src, sink);     

  freeNetwork(N);                 
  return 0;
}
