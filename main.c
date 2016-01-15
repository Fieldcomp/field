/* 
 * File:   main.c
 * Author: Avishay
 *
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
    token *prev;
    char c = '0';
    int i;
    while (1) {               
        i = 0;
        char tkn_name[15];
        
        while ((c = fgetc(file)) != ' ' && c != EOF) {
            tkn_name[i++] = c;
        }
        tkn_name[i] = '\0';
        current->token_name = (char *) malloc(sizeof(tkn_name));
        strcpy(current->token_name, tkn_name);
        
        // printf("%s", current->token_name);
        
        if (feof(file))
            break;
        
        fgetc(file);
        fgetc(file);
        
        i = 0;
        char tkn_expr[50];
        while (((c = fgetc(file)) != '\n')) {
            tkn_expr[i++] = c;
        }
        tkn_expr[i] = '\0';
        current->token_literal = (char *) malloc(sizeof(tkn_expr));
        strcpy(current->token_literal, tkn_expr);           
        
        current->next = malloc(sizeof(token));
        prev = current;
        current = current->next;
    }
    free(current);
    prev->next = NULL; //egh for some reason I can't fix it without having the previous struct pointed
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
            
        } else
            //printf("%s did compile\n", nextT->token_name);
        
        nextT = nextT->next;
    }
}

int main(int argc, char** argv) {
    FILE *token_file = fopen("tokens.txt", "r");
    read_tokens(token_file);
    fclose(token_file);
    
    print_token_names();
    
    compile_regex();
    
    int check = regexec(&first->token_regex, "a", 0, NULL, 0);
    
    printf("trying %s-%s\n", first->token_name, first->token_literal);
    printf("%s", check ? "nope" : "yay!");
    
/* working compilation and execution code
    match = regcomp(&regex, "[a-c]", 0);
    if (match) {
        printf("not okay\n");
    } else
        printf("okay\n");
    
    match = regexec(&regex, "c", 0, NULL, 0);;
    
    if (!match) {
        puts("found!");
    } else if (match == REG_NOMATCH) {
        puts("no match found");
    }
*/
    return (EXIT_SUCCESS);
}

