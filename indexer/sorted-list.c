#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorted-list.h"



freqlist_T FLCreate(char *filename, char *tokename){
    
    freqlist_T fl = (freqlist_T)malloc(sizeof(struct freqlist));
    
    fl->filename = strdup(filename);
    fl->tokename = strdup(tokename);
    
    fl->freq = 1;
    
    fl->next = NULL;
    fl->parent = NULL;
    
    return fl;
}

void FLDestroy(freqlist_T fl){
    
    if(fl == NULL){
        return;
    }
    
    FLDestroy(fl->next);
    
    free(fl->tokename);
    free(fl->filename);
    
    free(fl);
    
}

freqlist_T swap(freqlist_T curr){
    
    printf("swap called\n");
    
    if(curr->parent != NULL){
        if(curr->freq > curr->parent->freq){
            if(curr->parent->parent == NULL){
                printf("swap 1 start\n");
                curr->parent->next = curr->next;
                curr->next = curr->parent;
                curr->parent = curr->parent->parent;
                curr->next->parent = curr;
                printf("swap 1 finish\n");
            }else{
                printf("swap 2 start\n");
                curr->parent->next = curr->next;
                curr->next = curr->parent;
                curr->parent = curr->parent->parent;
                curr->parent->next = curr;
                curr->next->parent = curr;
                curr = swap(curr);
                printf("swap 2 finish\n");
            }
        }
    }
    return curr;
    
}

freqlist_T FLInsert(freqlist_T head, char *filename, char *tokename){
    
    freqlist_T curr = head;
    
    while(curr!=NULL){
        
        if(strcmp(curr->filename, filename) == 0){
            curr->freq++;
            curr = swap(curr);
            return head;
        }else if(curr->next == NULL){
            curr->next = FLCreate(filename, tokename);
            curr->next->parent = curr;
            return head;
        }else{
            curr = curr->next;
        }
    }
    
    
    return NULL;
}

tokenlist_T TLCreate(char *filename, char *tokename){
    
    tokenlist_T tl = (tokenlist_T)malloc(sizeof(struct tokenlist));
    
    tl->tokename = strdup(tokename);
    
    tl->next = NULL;
    
    tl->freqlisthead = FLCreate(filename, tokename);
    
    return tl;
}

void TLDestroy(tokenlist_T tl){
    
    if(tl == NULL){
        return;
    }
    
    TLDestroy(tl->next);
    
    FLDestroy(tl->freqlisthead);
    
    free(tl->tokename);
    free(tl);
    
}

tokenlist_T TLInsert(tokenlist_T head, char *filename, char *tokename){
    
    if(head == NULL){
        head =  TLCreate(filename, tokename);
        return head;
    }
    
    tokenlist_T curr = head;
    tokenlist_T prev = head;
    
    while(curr != NULL){
        
        int result = strcmp(tokename, curr->tokename);
        
        if(result == 0){
            curr->freqlisthead = FLInsert(curr->freqlisthead, filename, tokename);
            return head;
        }else if(result < 0){
            
            tokenlist_T newlist = TLCreate(filename, tokename);
            
            if(curr == prev){
                head = newlist;
                newlist->next = curr;
                return head;
            }
            else{
                newlist->next = curr;
                prev->next = newlist;
                return head;
            }
        }
        
        if(curr == prev){
            curr = curr->next;
        }else{
            curr = curr->next;
            prev = prev->next;
        }
        
    }
    
    tokenlist_T newlist = TLCreate(filename, tokename);
    
    prev->next = newlist;
    
    return head;
    
}
