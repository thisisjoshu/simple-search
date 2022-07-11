//gcc -Wall -lm -std=c11 searchPagerank.c -o searchPagerank

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

void processData(int size, char **urlArr);
void sortByFreq(int *frequency, char **urlArr, int size);
void sortByPagerank(char **urlArr, int size);
void processDisplay(int *frequency, char **urlArr, int size);
void normalize(char *word);
void display(char **urls, int size);
int distFreq(int size, int *frequency);
int maxFreq(int size, int *frequency);

int main (int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Atleast 2 arguments expected: %s Query1 Query2...\n", argv[0]);
        exit(1);
    }
    
    char **queries = NULL;
    char **urls = NULL;
    
    int words = 0;
    int nUrls = 0;
    int i;
    for (i = 0; i < argc - 1; i++) {
        FILE *invertedIndex = fopen("invertedIndex.txt", "r");
        normalize(argv[i + 1]);
        
        queries = realloc(queries, (words + 1) * sizeof(char *));
        queries[words] = malloc(strlen(argv[i]) + 1);
        strcpy(queries[words++], argv[i + 1]);
        
        char searches[100];
        char new[100];
        while (fscanf(invertedIndex, "%s", searches) == 1) {
            if (strcmp(queries[i], searches) == 0) {
                while (fscanf(invertedIndex, "%99s%99[ \t\n]", searches, new) == 2) {
                    urls = realloc(urls, (nUrls + 1) * sizeof(char *));
                    urls[nUrls] = malloc(strlen(searches) + 1);
                    strcpy(urls[nUrls++], searches);
                    
                    char ch = '\n';
                    if (strchr(new, ch)) break;
                }
            }
        }
        fclose(invertedIndex);
    }
    
    processData(nUrls, urls);
    return 0;
}

//processing of url data with respect to search queries
void processData(int size, char **urlArr) {

    int freqArr[size]; 
    int i;
    for(i = 0 ; i < size; i++) freqArr[i] = -1;

    //count the frequency of urls
    int j, k;
    for (i = 0; i < size; i++) {
        k = 1; 
        for (j = i + 1; j < size; j++) {
            if (strcmp(urlArr[i], urlArr[j]) == 0) {
                k++; 
                freqArr[j] = 0;
            }   
        }
        if (freqArr[i] != 0) freqArr[i] = k; 
    }
    
    char **urls = NULL;
    int *freqs = malloc(size * sizeof(int));
    int count = 0;
    for (i = 0; i < size; i++) {
        if (freqArr[i] != 0) {
            urls = realloc(urls, (count + 1) * sizeof(char *));
            urls[count] = malloc(strlen(urlArr[i]) + 1);
            strcpy(urls[count], urlArr[i]);
            freqs[count] = freqArr[i];
            count++;
        }
    }
    
    sortByFreq(freqs, urls, count); 
    processDisplay(freqs, urls, count);
    
}

//sort urls by frequency
void sortByFreq(int *frequency, char **urlArr, int size) {
    char *tempUrl;
    int i, j, temp;
    for (i = 0; i < size; i++) {
        for (j = i + 1; j < size; j++) {
            if (frequency[i] < frequency[j]) {
                temp = frequency[i]; 
                frequency[i] = frequency[j];
                frequency[j] = temp;
                
                tempUrl = urlArr[i];
                urlArr[i] = urlArr[j];
                urlArr[j] = tempUrl;
            }
        }
    }
}


void sortByPagerank(char **urlArr, int size) {
    double *pageranks = malloc(size * sizeof(double));
    
    //read from pagerank file
    int i, count = 0;
    for (i = 0; i < size; i++) {
        FILE *pagerankList = fopen("pagerankList.txt", "r");
        char urlData[100];
        char new[100];
        while (fscanf(pagerankList, "%s", urlData) != EOF) {
            normalize(urlData);
            if (strcmp(urlData, urlArr[i]) == 0) {
                while (fscanf(pagerankList, "%99s%99[ \t\n]", urlData, new) == 2) {
                    char ch = '\n';
                    if (strchr(new, ch)) {
                        pageranks[count++] = atof(urlData); break;
                    }
                }
            }
        }
        fclose(pagerankList);
    }
    
    int j;
    char *tempUrl; double tempPagerank;
    for (i = 0; i < count; i++) {
        for (j = i + 1; j < count; j++) {
            if (pageranks[i] < pageranks[j]) {
                tempPagerank = pageranks[i];
                pageranks[i] = pageranks[j];
                pageranks[j] = tempPagerank;
                
                tempUrl = urlArr[i];
                urlArr[i] = urlArr[j];
                urlArr[j] = tempUrl; 
            }
        }
    }
    free(pageranks);
}

void processDisplay(int *frequency, char **urlArr, int size) {
    int numDist = distFreq(size, frequency);
    int start = 0;
    int numOutput = 0;
    char **tempUrl = NULL;
    char **out = NULL; 
    
    int i, j, counter, max;
    for (i = 0; i < numDist ; i++) {
        counter = 0;
        max = maxFreq(size, frequency);
        for (j = start; j < size; j++) {
            if (frequency[j] != max) {
                start = j; break;
            }
            tempUrl = realloc(tempUrl, (counter + 1) * sizeof(char *));
            tempUrl[counter] = malloc(strlen(urlArr[j]) + 1);
            strcpy(tempUrl[counter++], urlArr[j]);
            frequency[j] = -1;
        }
        
        sortByPagerank(tempUrl, counter);
        
        int k;
        for (k = 0; k < counter; k++) {
            out = realloc(out, (numOutput + 1) * sizeof(char *));
            out[numOutput] = malloc(strlen(tempUrl[k]) + 1);
            strcpy(out[numOutput++], tempUrl[k]);
        }
    }
    
    display(out, numOutput);
}



//Helper functions

//processing of string
void normalize(char *word) {
    // convert to lowercase
    for ( ; *word; ++word) *word = tolower(*word);
    
    //remove '.',',',':',';','?' from string
    if (word[strlen(word) - 1] == '.') word[strlen(word) - 1] = '\0';
    if (word[strlen(word) - 1] == ',') word[strlen(word) - 1] = '\0';
    if (word[strlen(word) - 1] == ';') word[strlen(word) - 1] = '\0';
    if (word[strlen(word) - 1] == ':') word[strlen(word) - 1] = '\0';
    if (word[strlen(word) - 1] == '?') word[strlen(word) - 1] = '\0';
}

//prints urls to stdout
void display(char **urls, int size) {
    int i;
    if (size > 30) { 
        for (i = 0; i < 30; i++) printf("%s\n", urls[i]);
    }else{
        for (i = 0; i < size; i++) printf("%s\n", urls[i]);
    }
}

// returns count of distinct frequencies
int distFreq(int size, int *frequency) {
    int count = 1; 
    int distinct; 
    int i, j;
    
    for (i = 1; i < size; i++) {
        distinct = 1;
        for (j = 0; distinct && j < i; j++) 
            if (frequency[i] == frequency[j]) distinct = 0;
        if (distinct) count++;
    }
    return count;
}

//returns maximum frequency
int maxFreq(int size, int *frequency) {
    int max = frequency[0];
    int i;
    
    for (i = 1; i < size; i++) {
        if (frequency[i] > max) max = frequency[i];
    }
    return max;
}

