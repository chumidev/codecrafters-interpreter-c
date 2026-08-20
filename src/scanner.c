#include "scanner.h"
#include "errors.h"
#include "token.h"

#include <stdio.h>

ErrorCodes scan_contents(char *contents, TokenArray *out) {
    if (!contents || !out) return ERROR_INVALID_INPUT;

    char c;
    int line = 0;
    ErrorCodes return_code = SUCCESS;
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

        case '=':
            if (*contents == '=') {
                add_token(out, (Token){TOKEN_EQUAL_EQUAL, "==", .literal = NULL, .line = line});
                contents++;
            } else add_token(out, (Token){TOKEN_EQUAL, "=", .literal = NULL, .line = line});
            break;

        case '!':
            if (*contents == '=') {
                add_token(out, (Token){TOKEN_BANG_EQUAL, "!=", .literal = NULL, .line = line});
                contents++;
            } else add_token(out, (Token){TOKEN_BANG, "!", .literal = NULL, .line = line});
            break;

        default:
            fprintf(stderr, "[line %d] Error: Unexpected character: %c\n", line + 1, c);
            return_code = ERROR_LEXICAL;
            break;
        }
    }
    add_token(out, (Token){TOKEN_EOF, "", .literal = NULL, .line = line});
    return return_code;
}
