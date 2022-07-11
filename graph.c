// Graph ADT
// Adjacency List Representation ... COMP2521 
// from week 5a lecture exercises - heavily modified

#include "graph.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


Graph newGraph(int nV) {
   assert(nV >= 0);
   int i;

   Graph g = malloc(sizeof(GraphRep));
   assert(g != NULL);
   g->nV = nV;
   g->nE = 0;
   g->vertex = malloc(nV*sizeof(char *));
   // allocate memory for array of lists
   g->edges = malloc(nV * sizeof(List));
   assert(g->edges != NULL);
   for (i = 0; i < nV; i++)
      g->edges[i] = NULL;

   return g;
}

void insertEdge(Graph g, Edge e) {
   assert(g != NULL);
   if (!inLL(g->edges[e.id], e.w) && e.w != NULL) {   // edge e not in graph
      g->edges[e.id] = insertLL(g->edges[e.id], e.w);
      g->nE++;
   }
   
}

void removeEdge(Graph g, Edge e) {
   assert (g != NULL);
   if (inLL(g->edges[e.id], e.w)) {   // edge e in graph
      g->edges[e.id] = deleteLL(g->edges[e.id], e.w);
      g->nE--;
   }
}

void showGraph(Graph g) {
    assert(g != NULL);
    int j;
    printf("Number of vertices: %d\n", g->nV);
    printf("Number of edges: %d\n\n", g->nE);
    
    for (j = 0; j < g->nV; j++) {
        printf("Vertex: %s\n", g->vertex[j]);
	    printf("connects to\n");
        showLL(g->edges[j]);
        printf("\n");
    }
}


void freeGraph(Graph g) {
   assert(g != NULL);
   int i;

   for (i = 0; i < g->nV; i++)
      freeLL(g->edges[i]);

   free(g);
}
