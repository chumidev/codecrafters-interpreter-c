#pragma once

#include <stdbool.h>

typedef enum {
    TOKEN_EOF,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
} TokenType;

typedef struct {
    TokenType type;
    const char *lexeme;
    union {
        const char *literal;
        int literal_num;
    };
    unsigned int line;
} Token;

typedef struct {
    Token *tokens;
    int count;
    int capacity;
} TokenArray;

void free_array(TokenArray *array);
bool add_token(TokenArray *array, Token token);
void print_token_array(TokenArray *array);

void print_token(Token *token);
const char *tokentype_to_str(TokenType type);
