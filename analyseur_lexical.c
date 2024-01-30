//
// Created by Arsan Abdi  on 28/01/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct {
    char* type;
    char* value;
} Token;

Token* new_token(char* type, char* value) {

    Token *token = (Token*) malloc(sizeof(Token));
    token -> type = strdup(type);
    token -> value = strdup(value);
    return token;
}

void print_token(Token *token) {
    printf("Type: %s, Value: %s\n", token -> type, token -> value);
}

void free_token(Token *token) {
    free(token -> type);
    free(token -> value);
    free(token);
}

void lexical_analysis(const char* code) {
    while (*code != '\0') {
        if (isdigit(*code)) {

            char number[256];
            int i = 0;
            while (isdigit(*code)) {
                number[i++] = *code++;
            }
            number[i] = '\0';
            Token *token = new_token("Number", number);
            print_token(token);
            free_token(token);
        }
        else if (isalpha(*code)) {

            char word[256];
            int i = 0;
            while (isalpha(*code)) {
                word[i++] = *code++;
            }
            word[i] = '\0';
            Token *token = new_token("Word", word);
            print_token(token);
            free_token(token);
        }
        else if (ispunct(*code)) {

            char symbol[2] = {*code++, '\0'};
            Token *token = new_token("Symbol", symbol);
            print_token(token);
            free_token(token);
        }
        else {

            code++;
        }
    }
}

int main() {
    const char* code = "MOV R1, 45; ADD R2, R1";
    lexical_analysis(code);
    return 0;
}
