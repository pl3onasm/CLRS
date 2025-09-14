/* 
  Generic graph implementation
  Description: 
    A graph G = (V, E) is implemented as a hash table, 
    where the keys are the vertices and the values are
    the adjacency lists. The adjacency lists are doubly
    linked lists of edges.
    The graph can be directed or undirected, and the
    edges can be weighted or unweighted.
    Nodes and edges can have different flavors and are
    defined in vertex.h and edge.h. You can add your own
    vertex and edge types by modifying these headers.
    You could, for example, add a vertex with a void 
    pointer to satellite data.
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "../../htables/multi-value/htable.h"
#include "edge.h"
#include "vertex.h"

  // graph types
typedef enum { DIRECTED, UNDIRECTED } graphType;
typedef enum { WEIGHTED, UNWEIGHTED } weightType; 

  // graph data structure
typedef struct {
  htable *V;          // hash table of vertices
  size_t nEdges;      // number of edges in the graph
  graphType type;     // directed or undirected, 
                      // set to directed by default
  weightType weight;  // weighted or unweighted
  edge *e;            // dummy edge for lookup
  dll *adjList;       // dummy adjacency list
  vertex *u;          // dummy vertex for lookup
  vertex *v;          // dummy vertex for lookup  
  vertex *z;          // dummy vertex for iteration
} graph;


  // Creates a new graph; requires the initial capacity
  // and type of the edges (weighted/unweighted)
  // By default, the graph is directed 
graph *newGraph (size_t capacity, weightType weight);

  // Sets the graph type to undirected
  // Should be called before adding any edges
void setUndirected(graph *G);

  // Deallocates the graph
void freeGraph(graph *G);

  // Sets the pointer to the adjList of a vertex
  // If the vertex has no adj list or is not in the graph,
  // the pointer is set to NULL
  // Returns true if the source vertex is in the graph
bool hasAdjList(graph *G, vertex *v, dll **adjList);

  // Gets the adjacency list of a vertex
dll *getNeighbors(graph *G, vertex *v);

  // Gets the adjacency list of a vertex by label
dll *getNeighborsL(graph *G, char *label);

  // Gets the vertex given its label
vertex *getVertex(graph *G, char *label);

  // Adds a vertex with given label to the graph; 
  // the vertex is not added if it already exists
void addVertex(graph *G, char *label);

  // Same as addVertex, but returns a 
  // pointer to the vertex
vertex *addVertexR(graph *G, char *label);

  // Returns true if the vertex exists in the graph
bool hasVertex(graph *G, char *label);

  // Gets the edge between two vertices;
  // returns NULL if the edge does not exist
edge *getEdge(graph *G, vertex *from, vertex *to);

  // Adds an unweighted edge to the destination 
  // vertex's adjList
  // In an undirected graph, the edge is also 
  // added to the destination vertex's adjList
void addEdge(graph *G, vertex *from, vertex *to);

  // Same as addEdge, but for a weighted edge
void addEdgeW(graph *G, vertex *from, vertex *to, 
              double weight);

  // Adds an unweighted edge given the labels of 
  // the source and destination vertices
  // In an undirected graph, the edge is also
  // added to the destination vertex's adjList
void addEdgeL(graph *G, char *from, char *to);

  // Same as addEdgeL, but for a weighted edge
void addEdgeWL(graph *G, char *from, char *to, 
               double weight);

  // Adds the vertices if they do not exist, and
  // adds the edge between them with the given weight
  // In an undirected graph, the edge is also added
  // to the destination vertex's adjList
void addVandE(graph *G, char *from, char *to);

  // Same as addVandE, but for a weighted edge
void addVandEW(graph *G, char *from, char *to, 
               double weight);

  // Returns true if the edge exists in the graph
bool hasEdge(graph *G, vertex *from, vertex *to);

  // Returns true if the edge exists in the graph
  // given the labels of the source and destination 
bool hasEdgeL(graph *G, char *from, char *to);

  // Removes an edge from source vertex's adjList
  // If the graph is undirected, it is also removed 
  // from the destination vertex's adjList
void delEdge(graph *G, vertex *from, vertex *to);

  // Removes an edge from the graph given the 
  // labels of the source and destination 
void delEdgeL(graph *G, char *from, char *to);

  // Shows the graph
void showGraph(graph *G);

  // Shows a vertex and its adjacency list
void showVertex(graph *G, vertex *v);

  // Shows a vertex and its adjList given the label
void showVertexL(graph *G, char *label);

  // Reads a graph from standard input
void readGraph(graph *G);

  // Returns a copy of the graph
graph *copyGraph(graph *G);

  // Returns the transposed graph of G
  // This is a copy of G with all edges reversed
graph *transposeGraph(graph *G);

  // Returns an array of the vertices in the graph
vertex **getVertices(graph *G);

  // Returns an array of the vertices in the graph
  // sorted by their labels
vertex **sortVertices(graph *G);

  // Returns the number of vertices in the graph
static inline size_t nVertices(graph *G) {
  return htSize(G->V);
}

  // Returns the number of edges in the graph
static inline size_t nEdges(graph *G) {
  return G->nEdges;
}

  // Returns the out-degree of a vertex
static inline size_t outDegree(graph *G, vertex *v) {
  return dllSize(getNeighbors(G, v));
}

  // Same as above, but by label
static inline size_t outDegreeL(graph *G, char *label) {
  return dllSize(getNeighborsL(G, label));
}

  // Returns the in-degree of a vertex
static inline size_t inDegree(graph *G, vertex *v) {
  return v ? v->inDegree : 0;
}

  // Same as above, but by label
static inline size_t inDegreeL(graph *G, char *label) {
  vertex *v = getVertex(G, label);
  return v ? v->inDegree : 0;
}

  // Returns the degree of a vertex
static inline size_t degree(graph *G, vertex *v) {
  if (G->type == UNDIRECTED)
    return outDegree(G, v);
  return outDegree(G, v) + inDegree(G, v);
}

  // Returns the degree of a vertex given its label
static inline size_t degreeL(graph *G, char *label) {
  vertex *v = getVertex(G, label);
  return v ? degree(G, v) : 0;
}

  // Determines if the vertex is isolated
static inline bool isIsolated(graph *G, vertex *v) {
  return degree(G, v) == 0;
}

  // Same as above, but by label
static inline bool isIsolatedL(graph *G, char *label) {
  return degreeL(G, label) == 0;
}

  // Sets the label of the graph
static inline void setGLabel(graph *G, char *label) {
  htSetLabel(G->V, label);
}

  // Sets the string delimiter for showing 
  // the adjacency lists (default is ", ")
static inline void setAdjDelim(graph *G, char *adjDelim) {
  htSetValDelim(G->V, adjDelim);
}

  // Returns the weight of an edge
static inline double getWeight(edge *e) {
  return e->weight;
}

  // Returns true if the graph is empty
static inline bool graphIsEmpty(graph *G) {
  return htIsEmpty(G->V);
}

  // Returns the first edge in the graph and
  // sets the edge iterator to the next edge
  // Pointer to from is set to the source vertex
  // Returns NULL if there are no edges
edge *firstE(graph *G, vertex **from);

  // Returns the current edge of the iteration
  // and sets the edge iterator to the next edge;
  // Pointer to from is set to the source vertex
  // Returns NULL if there are no more edges
edge *nextE(graph *G, vertex **from);

  // Returns the first vertex in the graph
  // and sets the iterator to the next vertex
vertex *firstV(graph *G);

  // Returns the current vertex in the graph
  // and updates the iterator to the next vertex;
  // returns NULL if there are no more vertices
vertex *nextV(graph *G);
  
#endif // GRAPH_H_INCLUDED