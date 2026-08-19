#pragma once

#include <stdbool.h>

#include "token.h"

bool scan_contents(char *contents, TokenArray *out);
