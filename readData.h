#ifndef READDATA_H
#define READDATA_H

#include "set.h"
#include "graph.h"
#include "BSTree.h"

Set getCollection();
Graph getGraph(Set);
BSTree getInvertedList(Set);
void normalise(char *);

#endif
