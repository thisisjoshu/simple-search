// BSTree.c ... implementation of binary search tree ADT
// originally from COMP2521 lab- modified for this assignment

#include "BSTree.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>


typedef struct BSTNodeList {
    char  *url;
    BSTList next;
} BSTNodeList;

typedef struct BSTNode {
	char *word;
	BSTList urlList;
	BSTLink left, right;
} BSTNode;


void showBSTNodeBSTList(BSTLink node, FILE *out);
void BSTreeInfix(BSTree t, FILE *out);

// make a new node containing a value
static
BSTLink newBSTNode(char *word) {
	BSTLink new = malloc(sizeof(BSTNode));
	assert(new != NULL);
	new->word = (char*)calloc(strlen(word) + 1, sizeof(char*));
	strcpy(new->word, word);
	new->urlList = NULL;
	new->left = new->right = NULL;
	return new;
}

static
BSTList newBSTListNode(char *url) {
    BSTList new = malloc(sizeof(BSTNodeList));
    assert(new != NULL);
    new->url = malloc(sizeof(strlen(url) + 1));
    strcpy(new->url, url);
    new->next = NULL;
    return new;
}

// create a new empty BSTree
BSTree newBSTree() {
	return NULL;
}

// free memory associated with BSTree
void dropBSTree(BSTree t) {
	if (t == NULL) return;
	dropBSTree(t->left);
	dropBSTree(t->right);
	free(t);
}

// insert a new value into a BSTree
BSTree BSTreeInsert(BSTree t, char *word) {
	if (t == NULL) return newBSTNode(word);
	else if (strcmp(word, t->word) < 0)
		t->left = BSTreeInsert(t->left, word);
	else if (strcmp(word, t->word) > 0)
		t->right = BSTreeInsert(t->right, word);
	else // (word == t->word)
		/* don't insert duplicates */;
	return t;
}

void addBSTListNode(BSTLink node, char *url) {
    BSTList newListNode = newBSTListNode(url);
    BSTList curr = node->urlList;
    if (curr == NULL || strcmp(newListNode->url, curr->url) < 0) { 
        newListNode->next = curr; 
        node->urlList = newListNode;
        return;
    } 
    else { 
        if (strcmp(curr->url, newListNode->url) == 0) return;
        while (curr->next != NULL && strcmp(curr->next->url, newListNode->url) < 0) {
            curr = curr->next;
        } 
        if (curr->next != NULL && strcmp(curr->next->url, newListNode->url) == 0) return;

        newListNode->next = curr->next; 
        curr->next = newListNode; 
    } 
}

BSTNode *BSTreeFind(BSTree t, char *word) {
	if (t == NULL)
		return NULL;
	else if (strcmp(word, t->word) < 0)
		return BSTreeFind(t->left, word);
	else if (strcmp(word, t->word) > 0)
		return BSTreeFind(t->right, word);
	else
		return t;
}


void printBSTree(BSTree t, FILE *out) {
	if (t == NULL) return;
	printBSTree(t->left, out);
	showBSTNodeBSTList(t, out);
	printBSTree(t->right, out);
}

void showBSTNodeBSTList(BSTLink node, FILE *out) {
    fprintf(out, "%s", node->word);
    fprintf(out, " ");
    BSTList curr = node->urlList;
    while (curr != NULL) {
        fprintf(out, " %s", curr->url);
        curr = curr->next;
    }
    fprintf(out, "\n");
}
