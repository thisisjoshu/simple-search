// set.h ... interface to simple Set of Strings
// Written by John Shepherd, September 2015

#ifndef SET_H
#define SET_H

typedef struct SetRep *Set;

typedef struct Nodes *Link;

typedef struct Nodes {
	char *val; //url
	double  pagerank;
	int    outDegree;
	Link  next;
} Nodes;
	
typedef struct SetRep {
	int   nelems;
	Link  elems;
} SetRep;


// Function signatures

Set newSet();
void disposeSet(Set);
void insertInto(Set,char *);
void dropFrom(Set,char *);
int  isElem(Set,char *);
int  nElems(Set);
void showSet(Set);

#endif
