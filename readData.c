#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "readData.h"


//create a set(list) of URLS to process by reading data from "collection.txt"
Set getCollection() {
    char buf[100];
    FILE *fp;
    Set urlList = newSet();
    
    if ((fp = fopen("collection.txt", "r")) == NULL) {
        printf("Error! opening file");
        exit(1);         
    }
    
    while (fscanf(fp, "%s", buf) == 1) {
        insertInto(urlList, buf);
    }
    fclose(fp);
    return urlList;
}

//create a graph of URLS (from section 1 of each textfile)
Graph getGraph(Set urls) {
    int numUrls = nElems(urls);
    Graph new = newGraph(numUrls);
    
    Link curr = urls->elems;
    int i = 0;
    while (curr != NULL) {
        char *file = malloc(sizeof(char) * (strlen(curr->val) + 1));
        strcpy(file, curr->val);
        file = realloc(file, strlen(curr->val) + strlen(".txt") + 1);
        strcat(file, ".txt");
        
        FILE *fp;
        if ((fp = fopen(file, "r")) == NULL) {
            printf("Error! opening file");
            exit(1);         
        }
        
        char temp[100]; 
        fgets(temp, 100, fp);   //kip "#start section-1"
        char links[100];
        
        //update graph
        while (fscanf(fp, "%s", links) == 1 && strcmp(links, "#end") != 0) {
            
            Edge e;
            e.id = i;
            e.v = malloc(sizeof(strlen(curr->val + 1)));
            e.w = malloc(sizeof(strlen(links + 1)));
            new->vertex[e.id] = malloc(sizeof(e.v));
            strcpy(new->vertex[e.id], e.v);
            strcpy(e.v, file);
            strcpy(e.w, links);
            insertEdge(new, e);
            
            free(e.v);
            free(e.w);
        }
        
        Edge e;
        e.id = i;
        e.v = malloc(sizeof(strlen(curr->val + 1)));
        strcpy(e.v, curr->val);
        new->vertex[e.id] = malloc(sizeof(e.v));
        strcpy(new->vertex[e.id], e.v);
        free(e.v);
        
        fclose(fp);
        i++;
        curr = curr->next;
    }
    return new;
}

// returns BSTree that holds inverted list
BSTree getInvertedList(Set urls) {
    BSTree new = newBSTree();
    Link curr = urls->elems;
    int i = 0;
    while (curr != NULL) {
        char *file = malloc(sizeof(char) * (strlen(curr->val) + 1));
        strcpy(file, curr->val);
        file = realloc(file, strlen(curr->val) + strlen(".txt") + 1);
        strcat(file, ".txt");
        
        FILE *fp;
        if ((fp = fopen(file, "r")) == NULL) {
            printf("Error! opening file");
            exit(1);         
        }
        char temp[100]; 
        //a not so good way to skip "#start section-1"
        fgets(temp, 100, fp);   
        char word[100];
        
        //a not so good way to skip words in section 2
        while (fscanf(fp, "%s", word) == 1 && strcmp(word, "Section-2") != 0); 
        while (fscanf(fp, "%s", word) == 1 && strcmp(word, "#end") != 0) {
            normalise(word);
            new = BSTreeInsert(new, word);
            BSTLink node = BSTreeFind(new, word);
            addBSTListNode(node, curr->val);
        }
        fclose(fp);
        i++;
        curr = curr->next;
    }
    return new;
}

// Helper functions

void normalise(char *word) {
    // convert to lowercase
    for ( ; *word; ++word) *word = tolower(*word);
    
    //remove '.',',',':',';','?' from string
    if (word[strlen(word) - 1] == '.') word[strlen(word) - 1] = '\0';
    if (word[strlen(word) - 1] == ',') word[strlen(word) - 1] = '\0';
    if (word[strlen(word) - 1] == ';') word[strlen(word) - 1] = '\0';
    if (word[strlen(word) - 1] == ':') word[strlen(word) - 1] = '\0';
    if (word[strlen(word) - 1] == '?') word[strlen(word) - 1] = '\0';
}
