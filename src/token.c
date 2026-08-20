#include "token.h"
#include "errors.h"

#include <stdio.h>
#include <stdlib.h>

void free_token(Token *token) {
    if (!token) return;

    // TODO: Add check for future token types that might need it
    if (token->type == TOKEN_STRING) {
        free((void *)token->lexeme);
        free((void *)token->literal);
    }
}

void free_token_array(TokenArray *array) {
    if (!array) return;

    for (int i = 0; i < array->count; i++) free_token(&array->tokens[i]);

    free(array->tokens);
    array->tokens = NULL;
    array->capacity = 0;
    array->count = 0;
}

ErrorCodes add_token(TokenArray *array, Token token) {
    if (!array) return ERROR_INVALID_INPUT;

    if (array->count + 1 > array->capacity) {
        int capacity = array->capacity == 0 ? 256 : array->capacity * 2;
        Token *tmp = realloc(array->tokens, sizeof(*array->tokens) * capacity);
        if (!tmp) {
            fprintf(stderr, "Memory allocation failed\n");
            return ERROR_MALLOC;
        }
        array->tokens = tmp;
        array->capacity = capacity;
    }
    array->tokens[array->count++] = token;
    return SUCCESS;
}

const char *tokentype_to_str(TokenType type) {
    switch (type) {
    case TOKEN_EOF:         return "EOF";

    case TOKEN_LEFT_PAREN:  return "LEFT_PAREN";
    case TOKEN_RIGHT_PAREN: return "RIGHT_PAREN";
    case TOKEN_RIGHT_BRACE: return "RIGHT_BRACE";
    case TOKEN_LEFT_BRACE:  return "LEFT_BRACE";

    case TOKEN_COMMA:       return "COMMA";
    case TOKEN_DOT:         return "DOT";
    case TOKEN_MINUS:       return "MINUS";
    case TOKEN_PLUS:        return "PLUS";
    case TOKEN_SEMICOLON:   return "SEMICOLON";
    case TOKEN_STAR:        return "STAR";

    case TOKEN_EQUAL:       return "EQUAL";
    case TOKEN_EQUAL_EQUAL: return "EQUAL_EQUAL";
    case TOKEN_BANG:        return "BANG";
    case TOKEN_BANG_EQUAL:  return "BANG_EQUAL";

    case TOKEN_LESS:        return "LESS";
    case TOKEN_LESS_EQUAL:  return "LESS_EQUAL";
    case TOKEN_GREATER:     return "GREATER";
    case TOKEN_GREATER_EQUAL:
        return "GREATER_EQUAL";

    case TOKEN_SLASH:  return "SLASH";
    case TOKEN_STRING: return "STRING";
    }
}

void print_token(Token *token) {
    if (!token) return;
    // TODO: Add check for number token type later on so to print %d token->literal_num
    printf("%s %s %s\n", tokentype_to_str(token->type), token->lexeme, !token->literal ? "null" : token->literal);
}

void print_token_array(TokenArray *array) {
    if (!array) return;
    for (int i = 0; i < array->count; i++) {
        print_token(&array->tokens[i]);
    }
}
