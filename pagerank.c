// gcc -Wall -lm -std=c11 pagerank.c graph.c BSTree.c list.c set.c readData.c -o pagerank

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "readData.h"

typedef struct vertexData {
    char *url;
    double pagerank;
    double previousRank;
    int outD;
    
} vertexData;

typedef struct dataCollection {
    vertexData *data;
    int nU;         //#urls
} dataCollection;

void sortPagerank(dataCollection urlData, int numNodes);
double inlinkPopularity(int j, int k, Graph g);
double outlinkPopularity(int j, int k, Graph g);
void exportPagerankList(Set s);
Set calculatePageRank(Graph g, double d, double diffPR, double maxIterations, dataCollection urlData);
double inDegree(Graph g, char *url);
double outDegree(List L);
int getID(Graph g, List L, int i);

int main(int argc, char *argv[]) {

    if(argc != 4) {
        fprintf(stderr, "Usage: %s d diffPR maxIterations\n", argv[0]);
        return 1;
    }
    
    double d = atof(argv[1]);
    double diffPR = atof(argv[2]);
    double maxIterations = atof(argv[3]);

    Set list = getCollection();
    Graph g = getGraph(list);
    
    dataCollection urlData;
    urlData.nU = g->nV;
    urlData.data = malloc(urlData.nU * sizeof(vertexData));
    
    int i;
    for (i = 0; i < urlData.nU; i++) {
        int out = outDegree(g->edges[i]);
        urlData.data[i].outD = out;
    }
    
    Set pagerankList = calculatePageRank(g, d, diffPR, maxIterations, urlData);
    exportPagerankList(pagerankList);
    
    return 0;
}

//output list to pagerankList.txt
void exportPagerankList(Set s) {

    FILE *fp = fopen("pagerankList.txt", "w");
    Link curr = s->elems;
    while (curr != NULL) {
        fprintf(fp, "%s, %d, %.7f \n", curr->val, curr->outDegree, curr->pagerank); 
        curr = curr->next;
    }
    fclose(fp);
}

//calculate page rank
Set calculatePageRank(Graph g, double d, double diffPR, double maxIterations, dataCollection urlData) {
    Set list = newSet();
    int i;
    for (i = 0; i < g->nV; i++) urlData.data[i].pagerank = 1.0/g->nV;  //initialize all vertice's pagerank
    
    int iteration = 0;
    double sum = 0;
    double diff = diffPR;
    
    while ((iteration < maxIterations) && diff >= (diffPR)) {
        //diff = 0;
        int j, k;
        for (j = 0; j < g->nV; j++) {
            urlData.data[j].previousRank = urlData.data[j].pagerank;
            sum = 0;
            for (k = 0; k < g->nV; k++) {
                if (inLL(g->edges[k], g->vertex[j])) {
                    sum += urlData.data[k].pagerank * 
                           (inlinkPopularity(j, k, g)) * 
                           (outlinkPopularity(j, k, g));
                }
            }
            urlData.data[j].pagerank = ((1 - d)/g->nV) + (d * sum);
            diff += fabs(urlData.data[j].pagerank - urlData.data[j].previousRank);
        }
        iteration++;
    }
    
    
    //inserting data into list
    for (i = 0; i < g->nV; i++) {
        insertInto(list, g->vertex[i]);
        urlData.data[i].url = malloc(sizeof(strlen(g->vertex[i])));
        strcpy(urlData.data[i].url, g->vertex[i]);
    }
    
    sortPagerank(urlData, g->nV);
    Link curr;
    for (curr = list->elems, i = 0; curr != NULL; curr = curr->next, i++) {
        strcpy(curr->val, urlData.data[i].url);
        curr->pagerank = urlData.data[i].pagerank;
        curr->outDegree = urlData.data[i].outD;
    }
    return list;
}


// Calculate inlink popularity
double inlinkPopularity(int j, int k, Graph g) {
    double inlinkj = inDegree(g, g->vertex[j]);
    int kout = outDegree(g->edges[k]);
    double sum = 0;
    int i, id;
    
    for (i = 0; i < kout; i++) {
        id = getID(g, g->edges[k], i);
        sum += inDegree(g, g->vertex[id]); 
    }
    
    double inPopularity = 0;
    inPopularity = inlinkj / sum;
    
    return inPopularity;
}

// Calculate outlink popularity
double outlinkPopularity(int j, int k, Graph g) {
    double outlinkj = outDegree(g->edges[j]);
    int kout = outDegree(g->edges[k]);
    double sum = 0;
    int i, id;
    
    for (i = 0; i < kout; i++) {
        id = getID(g, g->edges[k], i);
        sum += outDegree(g->edges[id]);
        if (outDegree(g->edges[i]) == 0) sum += 0.5; //avoid dividing by zero
    }
    
    double outPopularity = 0;
    outPopularity = outlinkj / sum;
    
    return outPopularity;
}

void sortPagerank(dataCollection urlData, int numNodes) {
    int i;
    for (i = 1; i < numNodes; i++){
        int j;
        for (j = 0; j < numNodes - i; j++) {
            if (urlData.data[j].pagerank < urlData.data[j + 1].pagerank) {
                vertexData temp = urlData.data[j];
                urlData.data[j] = urlData.data[j + 1];
                urlData.data[j + 1] = temp;
            }
        }
    }
}

// Helper functions

double inDegree(Graph g, char *url) {
    double in = 0;
    int i;
    for (i = 0; i < g->nV; i++) {
        Node *curr;
        for (curr = g->edges[i]; curr != NULL; curr = curr->next) {
            if (strcmp(curr->url, url) == 0) in++;
        }
    }
    return in;
}

double outDegree(List L) {
    int out = 0;
    Node *curr = L;
    while (curr != NULL) {
        out++;
        curr = curr->next;
    }
    return out;
}

int getID(Graph g, List L, int i) {
    Node *curr = L;
    int j = 0;
    while (curr!= NULL && j < i) {
        curr = curr->next;
        j++;
    }
    
    for (j = 0; j < g->nV; j++)
        if (strcmp(g->vertex[j], curr->url) == 0)
            break;
            
    return j;
}
