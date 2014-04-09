#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "search.h"

FileNode_T FCreate(char* token, char* fname, int freq){

  FileNode_T node = (FileNode_T)malloc(sizeof(struct FileNode));
  
  node->fname = strdup(fname);
  node->token = strdup(token);
  node->freq = freq;
  node->next = NULL;

  //printf("Node Created with mark %s %s\n",fname,token);
  return node;
}

void FAdd(FileNode_T array[], FileNode_T node){

  int i;
  for(i=0; i<5; i++){
  	if(array[i] == NULL){
		array[i] = node;
		//printf("New file node %s added\n",node->fname);
		return;
	}
  	//printf("Comparing %s to %s\n",array[i]->fname,node->fname);
	if(strcmp(array[i]->fname, node->fname) == 0){
		FileNode_T curr = array[i];
		FileNode_T prev = array[i];
		int cmp = 0;
		while(curr != NULL){
			cmp = strcmp(curr->token,node->token);
			if(cmp > 0 && curr == prev){
				node->next = curr;
				array[i] = node;
				//printf("Node added at beginning\n");
				return;
			}
			else if(cmp > 0 && curr != prev){
				node->next = curr;
				prev->next = node;
				//printf("Node added in middle\n");
				return;
			}
			else if(curr->next == NULL){
				curr->next = node;
				//printf("Node added at the end\n");
				return;
			}
			
			if(curr == prev){
				curr = curr->next;
			}
			else{
				prev = prev->next;
				curr = curr->next;
			}
		}
	}
  }
}

void FDestroy(FileNode_T farray[]){
  
  int i;
  FileNode_T curr;
  FileNode_T prev;
  for(i=0; i<5; i++){
	curr = farray[i];
	while(curr != NULL){
		free(curr->fname);
		curr->fname = NULL;
		free(curr->token);
		curr->token = NULL;
		prev = curr;
		curr = curr->next;
		free(prev);
	}
	farray[i] = NULL;
  }
}

SearchNode_T SNCreate(char* token){

  SearchNode_T node = (SearchNode_T)malloc(sizeof(struct SearchNode));
  node->found = 0;
  node->token = strdup(token);
  node->next = NULL;
  return node;
}

Searcher_T SCreate(char* words){

  if(words == NULL){
  	printf("No search words\n");
  	return NULL;
  }

  Searcher_T searcher = (Searcher_T)malloc(sizeof(struct Searcher));
  int count = 0;
  char* ptr;
  ptr = words;
  while((ptr = strchr(ptr, ' ')) != NULL){
	count++;
	ptr++;
  }
  count++;

  char* token;
  int i;
  SearchNode_T temp;
  SearchNode_T adder;
  token = strtok(words, " ");
  temp = SNCreate(token);
  searcher->words = temp;
  adder = temp;
  while((token = strtok(NULL, " "))!= NULL){

	//printf("Token is %s\n", token);
	temp = SNCreate(token);
	adder->next = temp;
	adder = adder->next;
  }

  searcher->count = count;
  searcher->curr = NULL;

}

void SearchAll(FileNode_T array[], Searcher_T searcher){
  
  int i;
  int found = 0;
  int count = searcher->count;
  SearchNode_T ptr;
  for(i=0; i<5; i++){
	searcher->curr = array[i];
	while(searcher->curr != NULL){
		ptr = searcher->words;	
		while(ptr != NULL){
			if(ptr->found == 0){
				if(strcmp(ptr->token, searcher->curr->token) == 0){
					ptr->found = 1;
					searcher->count--;
					break;
				}			
			}
			ptr = ptr->next;
		}
		if(searcher->count == 0){
			printf("%s\n", array[i]->fname);
			found = 1;
			break;
		}
		searcher->curr = searcher->curr->next;
	}
	Reset(searcher, count);
  }
  if(found != 1){
  	printf("No files contain all search words\n");
  }
  return;
}

void SearchOr(FileNode_T array[], Searcher_T searcher){

  int i;
  int found;
  int count = searcher->count;
  SearchNode_T ptr;

  for(i=0; i<5; i++){
	found = 0;
  	searcher->curr = array[i];
  	while(searcher->curr != NULL){
  		ptr = searcher->words;
		while(ptr != NULL){
			if(strcmp(ptr->token, searcher->curr->token) == 0){
				found = 1;
				printf("%s\n", array[i]->fname);
				break;
			}
			ptr = ptr->next;
		}
		if(found == 1){
			found = 0;
			break;
		}
	searcher->curr = searcher->curr->next;
	}
  }
  return;
}

void Reset(Searcher_T searcher, int orcount){
  SearchNode_T ptr = searcher->words;
  while(ptr != NULL){
	if(ptr->found != 0){
		ptr->found = 0;
	}
	ptr = ptr->next;
  }
  searcher->count = orcount;
}

void SDestroy(Searcher_T searcher){
  SearchNode_T curr;
  SearchNode_T prev;
  curr = searcher->words;

  while(curr != NULL){
	prev = curr;
	curr = curr->next;
	free(prev->token);
	prev->token = NULL;
	free(prev);
  }
  searcher->words = NULL;
  searcher->curr = NULL;
  free(searcher);
}

void Print(FileNode_T array[], int index){
  FileNode_T ptr = array[index];
  while(ptr != NULL){
	printf("%s\n", ptr->token);
	ptr = ptr->next;
  }
}

int main(int argc, char** argv){

  FILE* file;
  if(argv[1] == NULL){
	printf("ERROR: No input file\n");
	return 0;
  }
  file = fopen(argv[1],"r");
  printf("File name is %s\n", argv[1]);

  FileNode_T farray[5] = {NULL, NULL, NULL, NULL, NULL};
  char* token = (char*)malloc(sizeof(char));
  char* name = (char*)malloc(sizeof(char));
  char* garbage = (char*)malloc(sizeof(char));
  int freq;
  FileNode_T temp;
  while(!feof(file)){

	fscanf(file,"%s %s",garbage,token);
	//printf("Garbage is: %s\nToken is: %s\n", garbage, token);

	fscanf(file,"%s",name);
	//printf("Fname is: %s\n",name); 
	
	while(strcmp(name, "</list>") != 0){
		fscanf(file,"%d",&freq);
		temp = FCreate(token,name,freq);
		FAdd(farray,temp);
		fscanf(file,"%s",name);
	}
  }
  free(token);
  free(name);
  free(garbage);

  char selection;
  char* words = (char*)malloc(sizeof(char));
  char* p;
  Searcher_T searcher;
  printf("(P)rint Index, Search (A)ll, Search (O)r, (Q)uit\n");
  printf("What would you like to do: ");
  scanf(" %c%*c", &selection);
  selection = tolower(selection);
  while(selection != 'q'){

	 switch(selection){
		
		case('p'):
			printf("Select a number from 1-5: ");
			int result;
			scanf("%d", &result);
			Print(farray,result-1);
			break;

		case('a'):
			printf("Enter search words: ");
			fgets(words, 1000*sizeof(char), stdin);
			if(words[0] == '\n'){
				printf("No search enter again\n");
				break;
			}
			if((p = strchr(words, '\n')) != NULL){
				*p = '\0';
			}
			p = words;
			for( ; *p; ++p) *p = tolower(*p);
			searcher = SCreate(words);
			SearchAll(farray, searcher);
			SDestroy(searcher);
			break;

		case('o'):
			printf("Enter search words: ");
			fgets(words, 1000*sizeof(char), stdin);
			if(words[0] == '\n'){
				printf("No search enter again\n");
				break;
			}
			if((p = strchr(words, '\n')) != NULL){
				*p = '\0';
			}
			p = words;
			for( ; *p; ++p) *p = tolower(*p);
			searcher = SCreate(words);
			SearchOr(farray, searcher);
			SDestroy(searcher);
			break;

		default:
			printf("Error: Incorrect input\n");
			break;

	}

  printf("What would you like to do: ");
  scanf(" %c%*c", &selection);
  selection = tolower(selection);
  }
  FDestroy(farray);
}


