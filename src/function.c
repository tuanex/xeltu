#include <stdio.h>

#include "debug.h"
#include "function.h"
#include "parser.h"

///Node* peek() {
///
///}

Result nilResult = {
	.type = NIL,
};

#define BINARY_OP(op, result, number1, number2) {\
	switch (op) { \
case BINOP_ADD: result = number1 + number2; break; \
case BINOP_SUB: result = number1 - number2; break; \
case BINOP_MUL: result = number1 * number2; break; \
case BINOP_DIV: result = number1 / number2; break; \
default: break; \
	} \
}

static Result traverse(Node* root) {
	switch (root->tag) {
case CONSTANT: {
	Result result = {
		.type = CONST,
		.F = root->value.leaf_val.F,
	};
	return result;
}
case BINARY: {
	Result left = traverse(root->value.binop_val.left);
	Result right = traverse(root->value.binop_val.right);

	if (left.type == CONST && right.type == CONST) {
		Result result;
		result.type = CONST;
		BINARY_OP(root->value.binop_val.op, result.F, left.F, right.F);
		return result;
	}
	break;
}
default: {}
	}
	return nilResult; 
}

Result compile(Node* root) {
	//disassembleRootNode(root, 0);
	Result result = traverse(root);
	return result;
}

void printResult(Result result) {
	switch (result.type) {
case CONST: {
	printf("%f\n", result.F);
}
default: 
	return;
	}
//printf("")
}
