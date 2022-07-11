// Graph ADT interface ... COMP2521 
// from week 5a lecture exercises - heavily modified

#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include "list.h"

typedef struct GraphRep *Graph;

// edges are pairs of vertices (end-points)
typedef struct Edge {
   int id;
   char *v; //src
   char *w; //dest
   
} Edge;

typedef struct GraphRep {
   List *edges;  // array of lists
   int   nV;     // #vertices
   int   nE;     // #edges
   char  **vertex;
} GraphRep;


Graph newGraph(int);
void  insertEdge(Graph, Edge);
void  removeEdge(Graph, Edge);
//bool  adjacent(Graph, Vertex, Vertex);
void  showGraph(Graph);
void  freeGraph(Graph);

#endif
