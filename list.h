// Linked list interface ... COMP2521 
//from week 5a lecture exercises - heavily modified

#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef struct Node *List;

typedef struct Node {
   char        *url;
   struct Node *next; 
} Node;

List insertLL(List, char*);
List deleteLL(List, char*);
bool inLL(List, char*);
void freeLL(List);
void showLL(List);

#endif
