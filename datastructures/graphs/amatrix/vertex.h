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


//===================================================================
// VERTEX TYPE 1  (application example: Johnson's algorithm)
#if defined (VERTEX_TYPE1)      

  typedef struct vertex {
    struct vertex *parent;    // ptr to the parent vertex
    char *label;              // the label of the vertex
    size_t idx;               // index of the vertex in the graph
    size_t inDegree;          // in-degree of the vertex
    size_t outDegree;         // out-degree of the vertex
    size_t bdist;             // distance from source in Bellman-Ford
    size_t ddist;             // distance from source in Dijkstra
  } vertex;

//===================================================================
// DEFAULT VERTEX TYPE  (application example: Floyd-Warshall)
#else     

  typedef struct vertex {   
    char *label;              // the label of the vertex
    size_t idx;               // index of the vertex in the graph
    size_t inDegree;          // in-degree of the vertex
    size_t outDegree;         // out-degree of the vertex
  } vertex;


#endif    // VERTEX TYPES

#endif    // VERTEX_H_INCLUDED