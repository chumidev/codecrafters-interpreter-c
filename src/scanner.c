#include "scanner.h"
#include "errors.h"
#include "token.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *get_substring(const char *start, size_t length) {
    char *substring = malloc(length + 1);
    if (!substring) return NULL;

    memcpy(substring, start, length);
    substring[length] = '\0';

    return substring;
}

ErrorCodes scan_contents(char *contents, TokenArray *out) {
    if (!contents || !out) return ERROR_INVALID_INPUT;

    char c;
    int line = 1;
    ErrorCodes return_code = SUCCESS;
    while ((c = *contents++) != '\0') {
        switch (c) {

        case ' ':
        case '\t':
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

        case '<':
            if (*contents == '=') {
                add_token(out, (Token){TOKEN_LESS_EQUAL, "<=", .literal = NULL, .line = line});
                contents++;
            } else add_token(out, (Token){TOKEN_LESS, "<", .literal = NULL, .line = line});
            break;

        case '>':
            if (*contents == '=') {
                add_token(out, (Token){TOKEN_GREATER_EQUAL, ">=", .literal = NULL, .line = line});
                contents++;
            } else add_token(out, (Token){TOKEN_GREATER, ">", .literal = NULL, .line = line});
            break;

        case '/':
            if (*contents == '/') {
                contents++;
                while (*contents != '\n' && *contents != '\0') contents++;

            } else add_token(out, (Token){TOKEN_SLASH, "/", .literal = NULL, .line = line});
            break;

        case '"': {
            char *start = contents - 1; // reset to starting '"' char
            int len = 1;

            while (*contents != '"' && *contents != '\0') {
                if (*contents == '\n') line++;
                len++;
                contents++;
            }
            if (*contents == '\0') {
                fprintf(stderr, "[line %d] Error: Unterminated string.\n", line);
                return_code = ERROR_LEXICAL;
                break;
            }

            // include the closing quote in total lexeme length
            len++;
            contents++;

            char *lexeme = get_substring(start, len);
            char *literal = get_substring(start + 1, len - 2);

            add_token(out, (Token){TOKEN_STRING, lexeme, .literal = literal, .line = line});
            break;
        }

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
            char *start = contents - 1;
            int len = 1;
            double number;

            // TODO: Add checks for number length. Big string numbers cannot be converted into real number without using big floats

            // Whole part
            while (isdigit(*contents)) {
                len++;
                contents++;
            }

            if (*contents == '.') {
                len++;
                contents++;

                if (!isdigit(*contents)) {
                    fprintf(stderr, "[line %d] Error: Wrong number formatting\n", line);
                    return_code = ERROR_LEXICAL;
                } else {
                    // Decimal part
                    while (isdigit(*contents)) {
                        len++;
                        contents++;
                    }
                }
            }

            char *lexeme = get_substring(start, len);
            if (sscanf(lexeme, "%lf", &number) != 1) {
                fprintf(stderr, "[line %d] Error: Wrong number formatting\n", line);
                return_code = ERROR_LEXICAL;
            }

            add_token(out, (Token){TOKEN_NUMBER, lexeme, .literal_num = number, .line = line});
            break;
        }

        default:
            fprintf(stderr, "[line %d] Error: Unexpected character: %c\n", line, c);
            return_code = ERROR_LEXICAL;
            break;
        }
    }
    add_token(out, (Token){TOKEN_EOF, "", .literal = NULL, .line = line});
    return return_code;
}
