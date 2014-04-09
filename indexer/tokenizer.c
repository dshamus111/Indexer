/*
 * tokenizer.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include "sorted-list.h"

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

struct TokenizerT_ {
    
    char *ts; /*string containing the tokens*/
    
    int tsLen; /*length of the token string*/
    
    int tsIndex; /*holds the index for the token string*/
    
};

typedef struct TokenizerT_ TokenizerT;

/*
 * TKCreate creates a new TokenizerT object for a given set of separator
 * characters (given as a string) and a token stream (given as a string).
 *
 * TKCreate should copy the two arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate(char *ts) {
    
    TokenizerT *tk = (TokenizerT*)malloc(sizeof(TokenizerT));
    
    /* Initializes the separator and token string with contents of argv[1]
     * and argv[2] respectivaly. strdup also allocates memory
     */
    tk->ts = strdup(ts);
    
    /* Initializes the lengths of the two strings*/
    tk->tsLen = (int)strlen(tk->ts);
    
    tk->tsIndex = 0; /*Initilizes token string index*/
    
    return tk;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy(TokenizerT *tk){
    
    /* Calls free on the strings in the Tokenizer and then calls free on
     * the Tokenizer itself.
     */
    free(tk->ts);
    free(tk);
    
}

/* chechChar takes a string and index from TKGetNextToken and adds the proper
 * hex ASCII code for the escape character found at the index given.
 */

char *checkChar(char *string, int index){
    
    /* If the char at the index is an escape character, its corrosponding
     * hex value is returned
     */
    if(string[index] == '\a'){
        return "[0x07]";
    }else if(string[index] == '\b'){
        return "[0x08]";
    }else if(string[index] == '\f'){
        return "[0x0c]";
    }else if(string[index] == '\r'){
        return "[0x0d]";
    }else if(string[index] == '\n'){
        return "[0x0a]";
    }else if(string[index] == '\t'){
        return "[0x09]";
    }else if(string[index] == '\v'){
        return "[0x0b]";
    }else if(string[index] == '\\'){
        return "[0x5c]";
    }else if(string[index] == '\"'){
        return "[0x22]";
    }
    
    return NULL; /*If no escape character is found, return NULL*/
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char is_delimiter(char character) {
	
	/*
	 * Description: determines if a particular character is a member of the set of delimiters
	 * Parameters: character to be compared, string of delimiters
	 * Modifies: Nothing
	 * Returns: 1 if character is a delimiter, 0 if it is not
	 */
	
	
	if(character < 48 || character > 57){
        if(character < 97 || character > 122){
            return 1;
        }
    }
    
	return 0;
}

char *TKGetNextToken(TokenizerT *tk) {
    
    char *token = (char*)(malloc(sizeof(char)));
     
    int i;/*keeps track of index in token stream*/
    i = tk->tsIndex;
    while(is_delimiter(tk->ts[i]) == 1 && tk->tsIndex != tk->tsLen){
	i++;
	tk->tsIndex = i;
    }
    /* Outer loop goes through the token steam and the inner loop goes
     * through the delimeter string and checks for any delimiter characters
     * in the token stream
     */
    for(i; i < tk->tsLen; i++){
        
        if(is_delimiter(tk->ts[i]) == 1){
            
            /* If delimeter is found in the token stream, return the token
             * created so far and updates the Index.
             */
            tk->tsIndex = i+1;
            return token;
            
        }
        token[strlen(token)] = tk->ts[i];
        token[strlen(token)] = 0;
        
        
        tk->tsIndex = i+1; /*Update index*/
        
    }
    
    
    tk->tsIndex = tk->tsLen; /*If the end of the string is reached, make index equal
                              *to length in order to end while loop.
                              */
    
    return token; /* return the final token*/
}

/* replaceChar takes the string from initParse, an index, and a character as
 * arguements. If the character to be replaced is just a backslash, the backslash
 * is removed. If the character to be replaced is an escape character the two
 * characters are replaced by a single escape character.
 */

char *replaceChar(char *string, int index, char c){
    
    /* If c is an escape character, replace the backslash with the escape character
     * and move the rest of the string back by one character. If c is the 0 character
     * move rest of the string back by two characters.
     */
    if(c != '0'){
        string[index] = c;
        char *ptr = &string[index + 1];
        char *ptr2 = &string[index + 2];
        
        memmove(ptr, ptr2, (int)strlen(ptr2));
        string[(int)strlen(string)-1] = 0;
    }else{
        char *ptr = &string[index];
        char *ptr2 = &string[index + 1];
        
        memmove(ptr, ptr2, (int)strlen(ptr2));
        string[(int)strlen(string)-1] = 0;
    }
    
    
    return string; /*return the updated string*/
}

/* initParse goes through the delimiter string and the token stream in order
 * to find escape characters. If a backslash is found, the next character is checked.
 * If the backslash and the next character create an escape character, replaceChar
 * is called and the length of the string is updated. If the next character doesn't
 * create an escape character, replaceChar is still called but excutes differently.
 */

char *initParse(char *string){
    
    int i;
    int length = (int)strlen(string);
    
    /* Goes through the string and looks for a backslash character and if it is found
     * call replaceChar with the proper arguements depnding on the following character.
     */
    for(i = 0; i < length; i++){
        
        if(string[i] == '\\'){
            if(string[i+1] == 'a'){
                
                string = replaceChar(string, i, '\a');
                length = (int)strlen(string);
                
            }else if(string[i+1] == 'b'){
                
                string = replaceChar(string, i, '\b');
                length = (int)strlen(string);
                
            }else if(string[i+1] == 'f'){
                
                string = replaceChar(string, i, '\f');
                length = (int)strlen(string);
                
            }else if(string[i+1] == 'r'){
                
                string = replaceChar(string, i, '\r');
                length = (int)strlen(string);
                
            }else if(string[i+1] == 'n'){
                
                string = replaceChar(string, i, '\n');
                length = (int)strlen(string);
                
            }else if(string[i+1] == 't'){
                
                string = replaceChar(string, i, '\t');
                length = (int)strlen(string);
                
            }else if(string[i+1] == 'v'){
                
                string = replaceChar(string, i, '\v');
                length = (int)strlen(string);
                
            }else if(string[i+1] == '\"'){
                
                string = replaceChar(string, i, '\"');
                length = (int)strlen(string);
                
            }else if(string[i+1] == '\\'){
                
                string = replaceChar(string, i, '\\');
                length = (int)strlen(string);
                
            }else{
                
                string = replaceChar(string, i, '0');
                length = (int)strlen(string);
            }
        }
    }
    
    return string; /*return the updated string*/
    
}

int isDir(const char *target){
    struct stat statbuf;
    stat(target, &statbuf);
    return S_ISDIR(statbuf.st_mode);
}

tokenlist_T tokenize(char *file, tokenlist_T tl){
    
    char *filename = strdup(file);
    
    FILE *fp = fopen(filename, "r");
    if(fp == NULL){
        printf("ERROR: file doesn't exist\n");
        fclose(fp);
        return tl;
    }
    
    char c;
    char *ts = (char*)malloc(1000 * sizeof(char));
    int i = 0;
    
    
    while(!feof(fp)){
        
        c = fgetc(fp);
        c = tolower(c);
        
        ts[i] = c;
        ts[i+1] = 0;
        
        i++;
    }
    
    TokenizerT* tokenizer = TKCreate(ts);
	
	if(tokenizer == NULL) {
		printf("Error: unable to create tokenizer\n");
	}
	
	char* token = NULL;
	token = TKGetNextToken(tokenizer);
    
	while(strcmp(token, "") != 0) {
        
        
        tl = TLInsert(tl, filename, token);
        
		printf("%s\n", token);
        
        token = TKGetNextToken(tokenizer);
	}
    return tl;
}

tokenlist_T dirPass(tokenlist_T tl, char *file){
    
    char *filename = strdup(file);
    
    struct dirent *pDirent;
    DIR *pDir;
    
    pDir = opendir(filename);
    if (pDir == NULL) {
        int a = strcmp(filename, ".");
        int b = strcmp(filename, "..");
        int c = strcmp(filename, ".DS_Store");
        
        if(a != 0 && b != 0 && c != 0){
            
            printf("%s is not a directory\n", filename);
            tl = tokenize(filename, tl);
        }
        return tl;
        
        closedir(pDir);
    }
    
    while ((pDirent = readdir(pDir)) != NULL) {
        int a = strcmp(pDirent->d_name, ".");
        int b = strcmp(pDirent->d_name, "..");
        int c = strcmp(pDirent->d_name, ".DS_Store");
        
        if(a != 0 && b != 0 && c != 0){
            
            char *path = strdup(filename);
            strcat(path, "/");
            strcat(path, pDirent->d_name);
            
            
            printf ("[%s]\n", pDirent->d_name);
            tl = dirPass(tl, path);
        }
    }
    return tl;
}

void writeFile(tokenlist_T tl, char *writefile){
    
    FILE *wf = fopen(writefile, "w");
    
    while(tl != NULL){
        fprintf(wf, "<list> %s\n", tl->tokename);
        while(tl->freqlisthead != NULL){
            fprintf(wf, "%s %d ", tl->freqlisthead->filename, tl->freqlisthead->freq);
            tl->freqlisthead = tl->freqlisthead->next;
        }
        fprintf(wf, "\n</list>\n");
        tl = tl->next;
    }
    
    fclose(wf);
    
}

int exists(char *writefile){
    
    char *filename = ".";
    
    struct dirent *pDirent;
    DIR *pDir;
    
    pDir = opendir(filename);
    
    while ((pDirent = readdir(pDir)) != NULL) {
        
        if(strcmp(writefile, pDirent->d_name) == 0){
            
            return 1;
        }
    }
    closedir (pDir);
    return 0;
}

/*
 * main will have two string arguments (in argv[1] and argv[2]).
 * The first string contains the separator characters.
 * The second string contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {
    
    
    if (argc < 3) {
        printf ("Usage: ./index <inverted-index file name> <directory or file name>");
        return 1;
    }
    
    char *filename = strdup(argv[2]);
    char *writefile = strdup(argv[1]);
    
    
    if(exists(writefile) == 1){
        printf("%s already exists, rewrite file? Enter y or n\n", writefile);
        char c;
        scanf("%c", &c);
        if(c != 'y'){
            printf("File not overwritten, enter new output filename to continue.\n");
            return 0;
        }
        
    }
    
    
    tokenlist_T tl = NULL;
    
    tl = dirPass(tl, filename);
    
    writeFile(tl, writefile);
    
    
	return 0;
}
