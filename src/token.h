#pragma once

#include "errors.h"

typedef enum {
    TOKEN_EOF,

    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,

    TOKEN_COMMA,
    TOKEN_DOT,
    TOKEN_MINUS,
    TOKEN_PLUS,
    TOKEN_SEMICOLON,
    TOKEN_STAR,

    TOKEN_EQUAL,
    TOKEN_EQUAL_EQUAL,
    TOKEN_BANG,
    TOKEN_BANG_EQUAL,

    TOKEN_LESS,
    TOKEN_LESS_EQUAL,
    TOKEN_GREATER,
    TOKEN_GREATER_EQUAL,

    TOKEN_SLASH,

    TOKEN_STRING,
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
} TokenType;

typedef struct {
    TokenType type;
    const char *lexeme;
    union {
        const char *literal;
        double literal_num;
    };
    unsigned int line;
} Token;

typedef struct {
    Token *tokens;
    int count;
    int capacity;
} TokenArray;

void free_token_array(TokenArray *array);
ErrorCodes add_token(TokenArray *array, Token token);
void print_token_array(TokenArray *array);

void free_token(Token *token);
void print_token(Token *token);
const char *tokentype_to_str(TokenType type);
