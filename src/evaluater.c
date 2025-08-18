#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "debug.h"

#include "common.h"
#include "evaluater.h"
#include "table.h"

HashMap* localMap;

static Result errorResult(const char* fmt, ...) {
	Result result = {
		.type = RESULT_ERROR,
	};
	printf("RESULT_ERROR ");
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	printf("\n");
	return result;
}

static Result visitNode(Node*);
static Result visitConst(Node* root) {
#ifdef EVALUATER_DEBUG
	printf("visitConst\n");
#endif
	return errorResult("visitConst");
}

static Result visitVar(Node* root) {
#ifdef EVALUATER_DEBUG
	printf("visitVar\n");
#endif
	return errorResult("visitVar");
}

static Result visitLeaf(Value value) {
#ifdef EVALUATER_DEBUG
	printf("visitLeaf\n");
#endif
	Result result;
	if (value.type == VALUE_CONST) {
		result.type = RESULT_CONST;
		result.value = value;
		return result;
	}
	else if (value.type == VALUE_VAR) {
		Entry* entry = getHashMap(localMap, value.name);
		if (entry == NULL) return errorResult("Undeclared variable.");
		if (!entry->value.defined) return errorResult("Undefined variable.");
		result.type = RESULT_CONST;
		result.value = entry->value;
		return result;
	}
	
	return errorResult("visitLeaf");
}

static Result visitBinary(Node* root) {
#ifdef EVALUATER_DEBUG
	printf("visitBinary\n");
#endif
#define BINOP(op, dest, number1, number2) {\
	switch(op){ \
case BINOP_ADD: dest = number1 + number2; break; \
case BINOP_SUB: dest = number1 - number2; break; \
case BINOP_MUL: dest = number1 * number2; break; \
case BINOP_DIV: dest = number1 / number2; break; \
default: break; \
	} \
}
	if (root->binop_value.op == BINOP_ASS) {
		Result right = visitNode(root->binop_value.right);

		if (right.type != RESULT_CONST) { errorResult("Trying to assign non-constant");}

		if (insertHashMap(localMap, root->binop_value.left->value.name, right.value)) {
			//return (Result) { .type = RESULT_SUCCESS };
			Result result;
			result.type = RESULT_SUCCESS;
			return result;
		}
		else {
			return errorResult("Assignment not successful");
		}
	}

	Result result;
	result. type = RESULT_CONST;
	Result left = visitNode(root->binop_value.left);
	if (left.type != RESULT_CONST) 
		return errorResult("Trying to use undefined variable");
	Result right = visitNode(root->binop_value.right);
	if (right.type != RESULT_CONST) 
		return errorResult("Trying to use undefined variable");
	BINOP(root->binop_value.op, result.value.constant, 
			left.value.constant, right.value.constant);
	return result;
}

static Result visitNode(Node* root) {
#ifdef EVALUATER_DEBUG
	printf("visitNode\n");
#endif
	switch (root->type) {
case NODE_LEAF: {
	Result result = visitLeaf(root->value);
	return result;
}
case NODE_BINARY: {
	Result result = visitBinary(root);
	return result;
}
default:
	break;
	}
	return errorResult("visitNode");
}

Result evaluate(Node* root, HashMap* map) {
	localMap = map;

	Result result = visitNode(root);

#ifdef EVALUATER_DEBUG
	disassembleResult(result);
#endif

	return result;
}
