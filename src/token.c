#include "token.h"
#include "errors.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void free_token(Token *token) {
    if (!token) return;

    if (token->type == TOKEN_NUMBER) {
        free((void *)token->lexeme);
    } else if (token->type == TOKEN_STRING) {
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
    case TOKEN_NUMBER: return "NUMBER";
    }
}

void print_token(Token *token) {
    if (!token) return;

    if (token->type == TOKEN_NUMBER) {
        char num_literal_str[64];
        snprintf(num_literal_str, sizeof(num_literal_str), "%s", token->lexeme);

        if (strchr(num_literal_str, '.')) {
            size_t len = strlen(num_literal_str);

            // strip away repeated zeroes leaving one left
            while (num_literal_str[len - 1] == '0') {
                if (len >= 3 && num_literal_str[len - 2] == '.') {
                    break;
                }
                num_literal_str[--len] = '\0';
            }
        } else strcat(num_literal_str, ".0");

        printf("%s %s %s\n", tokentype_to_str(token->type), token->lexeme, num_literal_str);

    } else
        printf("%s %s %s\n", tokentype_to_str(token->type), token->lexeme, !token->literal ? "null" : token->literal);
}

void print_token_array(TokenArray *array) {
    if (!array) return;
    for (int i = 0; i < array->count; i++) {
        print_token(&array->tokens[i]);
    }
}
