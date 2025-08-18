#ifndef XELTU_EVALUATER_H
#define XELTU_EVALUATER_H

#include "common.h"
#include "parser.h"
#include "value.h"
#include "table.h"

typedef enum { RESULT_CONST, RESULT_VAR, RESULT_SUCCESS, RESULT_ERROR } ResultType;

typedef struct Result {
	ResultType type;

	Value value;
} Result;

Result evaluate(Node*, HashMap*);
void freeResult(Result*);

#endif
