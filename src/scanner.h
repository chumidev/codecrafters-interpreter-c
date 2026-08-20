#pragma once

#include <stdbool.h>

#include "errors.h"
#include "token.h"

Errors scan_contents(char *contents, TokenArray *out);
