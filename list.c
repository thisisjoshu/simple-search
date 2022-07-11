// Linked list implementation ... COMP2521 
// from week 5a lecture exercises - heavily modified

#include "list.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


Node *makeNode(char  *n) {
   Node *new = malloc(sizeof(Node));
   assert(new != NULL);
   new->url = malloc(sizeof(n));
   strcpy(new->url, n);
   new->next = NULL;
   return new;
}

List insertLL(List L, char *n) {
   if (inLL(L, n))
      return L;
   
   // add new node at the beginning
   Node *new = makeNode(n);
   new->next = L;
   return new;
}

List deleteLL(List L, char *n) {
   if (L == NULL)
      return L;
   if (strcmp(L->url, n)== 0)
      return L->next;

   L->next = deleteLL(L->next, n);
   return L;

}

bool inLL(List L, char *n) {
   if (L == NULL)
      return false;
   if (strcmp(L->url, n)== 0)
     return true;

   return inLL(L->next, n);
}

void showLL(List L) {
   if (L == NULL)
      putchar('\n');
   else {
      printf("%s ", L->url);
      showLL(L->next);
   }
}

void freeLL(List L) {
   if (L != NULL) {
      freeLL(L->next);
      free(L);
   }
}
