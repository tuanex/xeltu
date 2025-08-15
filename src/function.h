#ifndef FUNCTION_H
#define FUNCTION_H

#include "common.h"
#include "parser.h"

typedef struct {
	int count;
	int capacity;
	OpCode* codes;
} Function;

typedef struct {
	enum { CONST, VARIABLE, NIL } type;

	union {
		Node* root;
		float F;
	};
} Result;

Result compile(Node*);
void printResult(Result);

#endif
