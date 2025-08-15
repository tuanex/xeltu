#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "value.h"

Value makeValue(const char* source, TokenType type) {
	Value value;
	(void)type;
	value.type = FLOAT;
	value.F = atof(source);
	return value;
}
