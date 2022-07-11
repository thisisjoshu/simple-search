// BSTree.h ... interface to binary search tree ADT
// originally from COMP2521 lab- modified for this assignment

#ifndef BSTREE_H
#define BSTREE_H

#include <stdio.h>

typedef struct BSTNode *BSTree;
typedef struct BSTNode *BSTLink;
typedef struct BSTNodeList *BSTList;


BSTree newBSTree();
BSTree BSTreeInsert(BSTree, char *);
void dropBSTree(BSTree);
void printBSTree(BSTree, FILE *);
void showBSTNodeBSTList(BSTLink, FILE *);
void addBSTListNode(BSTLink, char*);
struct BSTNode *BSTreeFind(BSTree, char *);


#endif
