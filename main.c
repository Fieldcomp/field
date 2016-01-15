/* 
 * File:   main.c
 * Author: Avishay
 * it's the base idea, tho it doesn't work perfectly yet
 * Created on January 14, 2016, 10:08 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

/*
 * 
 */
typedef struct token token;

struct token {
    char *token_name;
    char *token_literal;
    regex_t token_regex;
    token *next;
};


regex_t regex;
int match;

token *first;

void read_tokens(FILE *file) {
    first = malloc(sizeof(token));
    token *current = first;
    token *prev; // needed to free useless token in the end
    char c = '0';
    int i;
    while (!feof(file)) {
        c = fgetc(file);
        
        if(c == EOF)
            break;
        
        char name[15], val[30];
        
        i = 0;
        while (c != '=') {
            name[i++] = c;
            c = fgetc(file);
        }
        name[i] = '\0';
        
        c = fgetc(file); //get rid of the equal sign
        
        i = 0;
        while (c != '\n') {
            if (c == '\\') //needed to de-escape escape 
                switch(c = fgetc(file)) {
                    case 'n':
                        c = '\n';
                        break;
                    case 't':
                        c = '\t';
                        break;
                    default:
                        val[i++] = "\\";
                }
            val[i++] = c;
            c = fgetc(file);
        }
        val[i-1] = '\0';
        
        current->token_name = malloc(strlen(name));
        strcpy(current->token_name, name);
        
        current->token_literal = malloc(strlen(val));
        strcpy(current->token_literal, val);
        
        current->next = malloc(sizeof(token));
        prev = current;
        current = current->next;
    }
    prev->next = NULL;
}

void print_token_names() {
    token *nextT = first;
    while (nextT != NULL) {
        printf("%s - %s\n", nextT->token_name, nextT->token_literal);
        nextT = nextT->next;
    }
}

void compile_regex() {
    token *nextT = first;
    int chk;
    while (nextT != NULL) {
        chk = regcomp(&nextT->token_regex, nextT->token_literal, 0);
        
        if (chk) {
            printf("%s didn't compile\n", nextT->token_name);
            
        } //else
          //printf("%s did compile\n", nextT->token_literal);
        
        
        nextT = nextT->next;
    }
}

int main(int argc, char** argv) { // TODO a function that frees all of the program's memory when it's done.. ERGH ERGH LINKED LIST
    FILE *token_file = fopen("tokens.txt", "r");
    read_tokens(token_file);
    fclose(token_file);
    
    //print_token_names();
    compile_regex();
    int check = regexec(&first->next->next->token_regex, "A75", 0, NULL, 0);
    printf("%s", check ? "nope" : "yay!");
    
    return (EXIT_SUCCESS);
}

