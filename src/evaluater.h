#ifndef XELTU_EVALUATER_H
#define XELTU_EVALUATER_H

#include "common.h"
#include "scanner.h"
#include "parser.h"

typedef enum { RESULT_CONST, RESULT_VAR, RESULT_ERROR } ResultType;

typedef struct Result {
	ResultType type;

	union {
		float const_result;

		struct {
		} var_result;
	};
} Result;

Result* evaluate(Node*);
void freeResult(Result*);

#endif
