#ifndef Search_search_h
#define Search_search_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct FileNode{

	int freq;
	char* fname;
	char* token;
	struct FileNode* next;

};

typedef struct FileNode* FileNode_T;

struct SearchNode{

	int found;
	char* token;
	struct SearchNode* next;

};

typedef struct SearchNode* SearchNode_T;

struct Searcher{

	int count;
	FileNode_T curr;
	SearchNode_T words;
};

typedef struct Searcher* Searcher_T;


FileNode_T FCreate(char* token, char* fname, int freq);

void FAdd(FileNode_T array[], FileNode_T node);

void FDestroy(FileNode_T farray[]);

SearchNode_T SNCreate(char* token);

Searcher_T SCreate(char* words);

void SearchAll(FileNode_T array[], Searcher_T searcher);

void SearchOr(FileNode_T array[], Searcher_T searcher);

void Reset(Searcher_T searcher, int orcount);

void SDestroy(Searcher_T searcher);

#endif
