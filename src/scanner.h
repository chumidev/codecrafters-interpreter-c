#pragma once

#include <stdbool.h>

#include "errors.h"
#include "token.h"

ErrorCodes scan_contents(char *contents, TokenArray *out);
