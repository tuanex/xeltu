#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#include "common.h"
#include "evaluater.h"

static Result* makeResult() {
	return malloc(sizeof(Result));
}

void freeResult(Result* result) {
	free(result);
}

static Result* evaluateNode(Node* root) {
#define BINOP(type, dest, number1, number2) \
	switch (type) { \
		case BINOP_ADD: dest = number1 + number2; break; \
		case BINOP_SUB: dest = number1 - number2; break; \
		case BINOP_MUL: dest = number1 * number2; break; \
		case BINOP_DIV: dest = number1 / number2; break; \
	}

	switch (root->type) {
case NODE_CONST: {
	Result* result = makeResult();
	result->type = RESULT_CONST;
	result->const_result = root->const_value;
	return result;
}
case NODE_BINARY: {
	Result* result = makeResult();
	Result* left = evaluateNode(root->binop_value.left);
	Result* right = evaluateNode(root->binop_value.right);
	result->type = RESULT_CONST;
	BINOP(root->binop_value.op, result->const_result, left->const_result, right->const_result);
	freeResult(left);
	freeResult(right);
	return result;
}
default: {
	Result* result = makeResult();
	result->type = RESULT_ERROR;
	return result;
}
	}
}

Result* evaluate(Node* root) {
	Result* result = evaluateNode(root);

#ifdef EVALUATER_DEBUG
	disassembleResult(result);
#endif

	return result;
}
