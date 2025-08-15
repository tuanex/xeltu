#ifndef VALUE_H
#define VALUE_H

#include "lexer.h"

typedef struct Value {
	enum { FLOAT, INTEGER } type;

	union {
		int I;
		float F;
	};
} Value;

Value makeValue(const char*, TokenType);

#endif
