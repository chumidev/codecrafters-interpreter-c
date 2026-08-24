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

// Checks if contents are equal to rest, and increments count by the length of the rest string.
// If contents != rest, increments count until it finds a non alpha or nondigit char (except '_').
// *out will be TOKEN_IDENTIFIER or type if contents == rest.
static ErrorCodes checkKeyword(const char *contents, int *count, const char *rest, TokenType type, TokenType *out) {
    if (!contents || !count || !rest || !out) return ERROR_INVALID_INPUT;

    int len = strlen(rest);
    // 2nd and 3rd conditions are to check it isn't part of a longer variable name
    if (len > 0 && memcmp(contents, rest, len) == 0 && !isalnum(contents[len]) && contents[len] != '_') {
        *count += len;
        *out = type;
        return SUCCESS;
    }
    while (isalnum(*contents) || *contents == '_') {
        (*count)++;
        contents++;
    }
    *out = TOKEN_IDENTIFIER;
    return SUCCESS;
}

ErrorCodes scan_contents(char *contents, TokenArray *out) {
    if (!contents || !out) return ERROR_INVALID_INPUT;

    // TODO: Delete char c variable and evalute everything with *contents. Don't post increment in the while or switchs conditions
    int line = 1;
    ErrorCodes return_code = SUCCESS;
    while (*contents != '\0') {
        switch (*contents) {

        case ' ':
        case '\t':
        case '\r':
            contents++;
            break;

        case '\n':
            line++;
            contents++;
            break;

        case '(':
            add_token(out, (Token){TOKEN_LEFT_PAREN, "(", .literal = NULL, .line = line});
            contents++;
            break;

        case ')':
            add_token(out, (Token){TOKEN_RIGHT_PAREN, ")", .literal = NULL, .line = line});
            contents++;
            break;

        case '{':
            add_token(out, (Token){TOKEN_LEFT_BRACE, "{", .literal = NULL, .line = line});
            contents++;
            break;

        case '}':
            add_token(out, (Token){TOKEN_RIGHT_BRACE, "}", .literal = NULL, .line = line});
            contents++;
            break;

        case ',':
            add_token(out, (Token){TOKEN_COMMA, ",", .literal = NULL, .line = line});
            contents++;
            break;

        case '.':
            add_token(out, (Token){TOKEN_DOT, ".", .literal = NULL, .line = line});
            contents++;
            break;

        case '-':
            add_token(out, (Token){TOKEN_MINUS, "-", .literal = NULL, .line = line});
            contents++;
            break;

        case '+':
            add_token(out, (Token){TOKEN_PLUS, "+", .literal = NULL, .line = line});
            contents++;
            break;

        case ';':
            add_token(out, (Token){TOKEN_SEMICOLON, ";", .literal = NULL, .line = line});
            contents++;
            break;

        case '*':
            add_token(out, (Token){TOKEN_STAR, "*", .literal = NULL, .line = line});
            contents++;
            break;

        case '=':
            if (contents[1] == '=') {
                add_token(out, (Token){TOKEN_EQUAL_EQUAL, "==", .literal = NULL, .line = line});
                contents++;
            } else add_token(out, (Token){TOKEN_EQUAL, "=", .literal = NULL, .line = line});
            contents++;
            break;

        case '!':
            if (contents[1] == '=') {
                add_token(out, (Token){TOKEN_BANG_EQUAL, "!=", .literal = NULL, .line = line});
                contents++;
            } else add_token(out, (Token){TOKEN_BANG, "!", .literal = NULL, .line = line});
            contents++;
            break;

        case '<':
            if (contents[1] == '=') {
                add_token(out, (Token){TOKEN_LESS_EQUAL, "<=", .literal = NULL, .line = line});
                contents++;
            } else add_token(out, (Token){TOKEN_LESS, "<", .literal = NULL, .line = line});
            contents++;
            break;

        case '>':
            if (contents[1] == '=') {
                add_token(out, (Token){TOKEN_GREATER_EQUAL, ">=", .literal = NULL, .line = line});
                contents++;
            } else add_token(out, (Token){TOKEN_GREATER, ">", .literal = NULL, .line = line});
            contents++;
            break;

        case '/':
            if (contents[1] == '/') {
                while (*contents != '\n' && *contents != '\0') contents++;
            } else {
                add_token(out, (Token){TOKEN_SLASH, "/", .literal = NULL, .line = line});
                contents++;
            }
            break;

        case '"': {
            char *start = contents;
            contents++;
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
            char *start = contents;
            int len = 0;

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
            char *endptr;
            double number = strtod(lexeme, &endptr);
            if (endptr == lexeme) {
                fprintf(stderr, "[line %d] Error: Wrong number formatting\n", line);
                return_code = ERROR_LEXICAL;
            }

            add_token(out, (Token){TOKEN_NUMBER, lexeme, .literal_num = number, .line = line});
            break;
        }

        default: {
            if (isalpha(*contents) || *contents == '_') {
                char *start = contents;
                int len = 0;
                TokenType type = TOKEN_IDENTIFIER;

                switch (*contents) {
                case 'a': checkKeyword(contents, &len, "and", TOKEN_AND, &type); break;
                case 'c': checkKeyword(contents, &len, "class", TOKEN_CLASS, &type); break;
                case 'e': checkKeyword(contents, &len, "else", TOKEN_ELSE, &type); break;
                case 'f':
                    switch (contents[1]) {
                    case 'a': checkKeyword(contents, &len, "false", TOKEN_FALSE, &type); break;
                    case 'o': checkKeyword(contents, &len, "for", TOKEN_FOR, &type); break;
                    case 'u': checkKeyword(contents, &len, "fun", TOKEN_FUN, &type); break;
                    default:  checkKeyword(contents, &(len), "", TOKEN_IDENTIFIER, &type); break;
                    }
                    break;
                case 'i': checkKeyword(contents, &len, "if", TOKEN_IF, &type); break;
                case 'n': checkKeyword(contents, &len, "nil", TOKEN_NIL, &type); break;
                case 'o': checkKeyword(contents, &len, "or", TOKEN_OR, &type); break;
                case 'p': checkKeyword(contents, &len, "print", TOKEN_PRINT, &type); break;
                case 'r': checkKeyword(contents, &len, "return", TOKEN_RETURN, &type); break;
                case 's': checkKeyword(contents, &len, "super", TOKEN_SUPER, &type); break;
                case 't':
                    switch (contents[1]) {
                    case 'h': checkKeyword(contents, &len, "this", TOKEN_THIS, &type); break;
                    case 'r': checkKeyword(contents, &len, "true", TOKEN_TRUE, &type); break;
                    default:  checkKeyword(contents, &(len), "", TOKEN_IDENTIFIER, &type); break;
                    }
                    break;
                case 'v': checkKeyword(contents, &len, "var", TOKEN_VAR, &type); break;
                case 'w': checkKeyword(contents, &len, "while", TOKEN_WHILE, &type); break;
                default:  checkKeyword(contents, &len, "", TOKEN_IDENTIFIER, &type); break;
                }

                contents = start + len;
                char *lexeme = get_substring(start, len);
                add_token(out, (Token){type, lexeme, .literal = NULL, .line = line});
            } else {
                fprintf(stderr, "[line %d] Error: Unexpected character: %c\n", line, *contents);
                return_code = ERROR_LEXICAL;
                contents++;
            }
            break;
        }
        }
    }
    add_token(out, (Token){TOKEN_EOF, "", .literal = NULL, .line = line});
    return return_code;
}
