#include "scanner.h"
#include "token.h"

#include <stddef.h>
#include <stdio.h>

bool scan_contents(char *contents, TokenArray *out) {
    int line = 0;
    char c;
    while ((c = *contents++) != '\0') {
        switch (c) {

        case ' ':
        case '\r':
            break;

        case '\n':
            line++;
            break;

        case '(':
            add_token(out, (Token){TOKEN_LEFT_PAREN, "(", .literal = NULL, .line = line});
            break;

        case ')':
            add_token(out, (Token){TOKEN_RIGHT_PAREN, ")", .literal = NULL, .line = line});
            break;

        case '{':
            add_token(out, (Token){TOKEN_LEFT_BRACE, "{", .literal = NULL, .line = line});
            break;

        case '}':
            add_token(out, (Token){TOKEN_RIGHT_BRACE, "}", .literal = NULL, .line = line});
            break;

        case ',':
            add_token(out, (Token){TOKEN_COMMA, ",", .literal = NULL, .line = line});
            break;

        case '.':
            add_token(out, (Token){TOKEN_DOT, ".", .literal = NULL, .line = line});
            break;

        case '-':
            add_token(out, (Token){TOKEN_MINUS, "-", .literal = NULL, .line = line});
            break;

        case '+':
            add_token(out, (Token){TOKEN_PLUS, "+", .literal = NULL, .line = line});
            break;

        case ';':
            add_token(out, (Token){TOKEN_SEMICOLON, ";", .literal = NULL, .line = line});
            break;

        case '*':
            add_token(out, (Token){TOKEN_STAR, "*", .literal = NULL, .line = line});
            break;

        default:
            fprintf(stderr, "Character not supported: '%c'\n", c);
            return false;
        }
    }
    add_token(out, (Token){TOKEN_EOF, "", .literal = NULL, .line = line});
    return true;
}
