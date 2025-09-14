/*
  Author: David De Potter
  License: MIT, see LICENSE file in repository root folder
  Description:
    Defines different flavors of a vertex for a graph.
    To use an augmented vertex structure, define the desired
    VERTEX_TYPE from the command line using a flag, e.g.:

    $ gcc -O2 -D VERTEX_TYPE1 ...
    
    If no flag is set, the default vertex structure is used,
    which only contains the label and in-degree.
*/


#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#define MAX_LABEL 50

//===================================================================
// VERTEX TYPE 1  (application example: DFS)
#if defined (VERTEX_TYPE1)      

  typedef enum {
    WHITE,
    GRAY,
    BLACK
  } clr;

  typedef struct vertex {
    struct vertex *parent;    // ptr to the parent vertex
    char label[MAX_LABEL];    // the label of the vertex 
    clr color;                // color of the vertex
    size_t dTime;             // discovery time
    size_t fTime;             // finish time
    size_t inDegree;          // in-degree of the vertex
  } vertex;

//===================================================================
// VERTEX TYPE 2  (application example: BFS, Dijkstra, Prim, 
//                 Bellman-Ford)
#elif defined (VERTEX_TYPE2)

  typedef struct vertex {
    struct vertex *parent;    // pointer to the parent vertex
    double dist;              // distance from the source vertex
    char label[MAX_LABEL];    // the label of the vertex
    size_t inDegree;          // in-degree of the vertex
  } vertex;

//===================================================================
// VERTEX TYPE 3  (application example: topological sort, SCC, 
//                 Hierholzer, Fleury)
#elif defined (VERTEX_TYPE3)

  typedef struct vertex {
    char label[MAX_LABEL];    // the label of the vertex
    bool visited;             // marks the vertex as visited
    size_t inDegree;          // in-degree of the vertex
  } vertex;

//===================================================================
// VERTEX TYPE 4  (application example: DAG shortest paths)
#elif defined (VERTEX_TYPE4)

  typedef struct vertex {
    char label[MAX_LABEL];    // the label of the vertex
    double dist;              // distance from the source vertex
    struct vertex *parent;    // pointer to the parent vertex
    size_t dTime;             // discovery time
    size_t fTime;             // finish time
    size_t inDegree;          // in-degree of the vertex
  } vertex;

//===================================================================
// VERTEX TYPE 5  (application example: Johnson)
#elif defined (VERTEX_TYPE5)

  typedef struct vertex {
    char label[MAX_LABEL];    // the label of the vertex
    size_t index;             // index of the vertex
    double bfDist;            // Bellman-Ford distance
    double dDist;             // Dijkstra distance
    size_t parent;            // index of the parent vertex
    size_t inDegree;          // in-degree of the vertex
  } vertex;

//===================================================================
// VERTEX TYPE 6  (application example: MCBM, Hopcroft-Karp)
#elif defined (VERTEX_TYPE6)

  typedef struct vertex {
    char label[MAX_LABEL];    // the label of the vertex
    struct vertex *match;     // vertex matched to this vertex
    size_t level;             // level in the level graph
    size_t inDegree;          // in-degree of the vertex
    enum { NIL, 
           LEFT, 
           RIGHT } type;      // vertex belongs to left or right set
  } vertex;

//===================================================================
// VERTEX TYPE 7  (application example: MCBM, Gale-Shapley)
#elif defined (VERTEX_TYPE7)

  typedef struct vertex {
    char label[MAX_LABEL];    // the label of the vertex
    struct vertex *match;     // vertex matched to this vertex
    size_t index;             // index in the lookup table
    size_t inDegree;          // in-degree of the vertex
    enum { NIL, 
           LEFT, 
           RIGHT } type;      // vertex belongs to left or right set
  } vertex;

//===================================================================
// VERTEX TYPE 8  (application example: MCBM, Hungarian)

#elif defined (VERTEX_TYPE8)

  typedef struct vertex {
    char label[MAX_LABEL];    // the label of the vertex
    struct vertex *match;     // vertex matched to this vertex
    struct vertex *parent;    // parent in the augmenting path
    size_t inDegree;          // in-degree of the vertex
    enum { NIL, 
           LEFT, 
           RIGHT } type;      // vertex belongs to left or right set
    double height;            // height of the vertex 
    double slack;             // minimum distance separating an 
                              // unexplored vertex in R from being 
                              // adjacent to an explored vertex in L
    struct vertex *minParent; // pointer to the parent vertex 
                              // that minimizes the slack
  } vertex;

//===================================================================
// DEFAULT VERTEX TYPE
#else     

  typedef struct vertex {   
    char label[MAX_LABEL];    // the label of the vertex
    size_t inDegree;          // in-degree of the vertex
  } vertex;


#endif    // VERTEX TYPES

#undef MAX_LABEL

#endif    // VERTEX_H_INCLUDED