#pragma once

#include <stdio.h>
typedef enum {
    SUCCESS = 0,

    ERROR_INVALID_INPUT,
    ERROR_MALLOC,
    ERROR_EOF = EOF,

    ERROR_LEXICAL = 65,
} ErrorCodes;
