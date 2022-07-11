//gcc -Wall -lm -std=c11 inverted.c graph.c list.c set.c readData.c BSTree.c -o inverted

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "readData.h"

void exportInvertedIndex(BSTree t);

int main() {
    Set list = getCollection();
    BSTree tree = getInvertedList(list);
    exportInvertedIndex(tree);

    return 0;
}

void exportInvertedIndex(BSTree t) {

    FILE *fp = fopen("invertedIndex.txt", "w");
    printBSTree(t, fp);
    fclose(fp);
}
