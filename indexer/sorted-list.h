#ifndef Indexer_sorted_list_h
#define Indexer_sorted_list_h

#include <stdlib.h>

struct freqlist{
    
    char *filename;
    int freq;
    
    struct freqlist *next;
    struct freqlist *parent;
    
    char *tokename;
};

typedef struct freqlist* freqlist_T;

struct tokenlist{
    
    char *tokename;
    
    struct tokenlist *next;
    
    freqlist_T freqlisthead;
};

typedef struct tokenlist* tokenlist_T;

freqlist_T FLCreate(char *filename, char *tokename);

void FLDestroy(freqlist_T fl);

freqlist_T swap(freqlist_T curr);

freqlist_T FLInsert(freqlist_T head, char *filename, char *tokename);

tokenlist_T TLCreate(char *filename, char *tokename);

void TLDestroy(tokenlist_T tl);

tokenlist_T TLInsert(tokenlist_T head, char *filename, char *tokename);

#endif
