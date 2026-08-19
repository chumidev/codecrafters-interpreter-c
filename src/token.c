#include "token.h"

#include <stdio.h>
#include <stdlib.h>

void free_array(TokenArray *array) {
    free(array->tokens);
    array->capacity = 0;
    array->count = 0;
}

bool add_token(TokenArray *array, Token token) {
    if (array->count + 1 > array->capacity) {
        int capacity = array->capacity == 0 ? 256 : array->capacity * 2;
        Token *tmp = realloc(array->tokens, sizeof(*array->tokens) * capacity);
        if (!tmp) {
            fprintf(stderr, "Memory allocation failed\n");
            return false;
        }
        array->tokens = tmp;
        array->capacity = capacity;
    }
    array->tokens[array->count++] = token;
    return true;
}

const char *tokentype_to_str(TokenType type) {
    switch (type) {
    case TOKEN_EOF:
        return "EOF";
        break;
    case TOKEN_LEFT_PAREN:
        return "LEFT_PAREN";
        break;
    case TOKEN_RIGHT_PAREN:
        return "RIGHT_PAREN";
        break;
    }
}

void print_token(Token *token) {
    if (!token) return;
    // Add check for number token type later on so to print %d token->literal_num
    printf("%s %s %s\n", tokentype_to_str(token->type), token->lexeme, !token->literal ? "null" : token->literal);
}

void print_token_array(TokenArray *array) {
    if (!array) return;
    for (int i = 0; i < array->count; i++) {
        print_token(&array->tokens[i]);
    }
}
