#include <stdio.h>

#include "debug.h"
#include "parser.h"
#include "scanner.h"

void disassembleToken(Token token) {
	switch (token.type) {
case TOKEN_LEFT_PAREN:
	printf("TOKEN_LEFT_PAREN\n");
	break;
case TOKEN_RIGHT_PAREN:
	printf("TOKEN_RIGHT_PAREN\n");
	break;
case TOKEN_PLUS:
case TOKEN_MINUS:
case TOKEN_STAR:
case TOKEN_SLASH:
	printf("TOKEN_arithmetic\n    ");
	break;
case TOKEN_COLON_EQUAL:
	printf("TOKEN_COLON_EQUAL\n");
	return;
case TOKEN_IDENTIFIER:
	printf("TOKEN_IDENTIFIER\n    ");
	break;
case TOKEN_NUMBER:
	printf("TOKEN_NUMBER\n    ");
	break;
case TOKEN_EOF:
	printf("TOKEN_EOF\n");
	return;
case TOKEN_ERROR:
	printf("TOKEN_ERROR\n    ");
	break;
default:
	printf("UNKNOWN_TOKEN\n    ");
	break;
	}
	for (int i = 0; i < token.length; i++) {
		printf("%c", token.start[i]);
	}
	printf("\n");
}

void disassembleTokenArray(TokenArray tokens) {
	printf("Debugging TokenArray\n");
	for (int i = 0; i < tokens.count; i++) {
		disassembleToken(tokens.tokens[i]);
	}
}


void disassembleUnCode(UnCode un, int level) {
	for (int i = 0; i < level; i++) {
		printf("    ");
	}
	switch (un) {
case UNOP_POS:
	printf("UNOP_POS\n");
	break;
case UNOP_NEG:
	printf("UNOP_NEG\n");
	break;
	}
}

void disassembleOpCode(OpCode op, int level) {
	for (int i = 0; i < level; i++) {
		printf("    ");
	}
	switch (op) {
case BINOP_ADD:
	printf("BINOP_ADD\n");
	break;
case BINOP_SUB:
	printf("BINOP_SUB\n");
	break;
case BINOP_MUL:
	printf("BINOP_MUL\n");
	break;
case BINOP_DIV:
	printf("BINOP_DIV\n");
	break;
case BINOP_ASS:
	printf("BINOP_ASS\n");
	break;
	}
}

void disassembleRootNode(Node* root, int level) {
	switch (root->type) {
case NODE_LEAF:
	for (int i = 0; i < level; i++) {
		printf("    ");
	}
	if (root->value.type == VALUE_CONST) { 
		printf("CONST %f\n", root->value.constant);
	}
	else {
		printf("VAR %d\n", root->value.defined);
	}
	break;
case NODE_UNARY:
	disassembleUnCode(root->unop_value.un, level);
	disassembleRootNode(root->unop_value.operand, level + 1);
case NODE_ERROR:
	break;
case NODE_BINARY:
	disassembleOpCode(root->binop_value.op, level);
	disassembleRootNode(root->binop_value.left, level + 1);
	disassembleRootNode(root->binop_value.right, level + 1);
	}
}


void disassembleResult(Result result) {
	switch (result.type) {
case RESULT_CONST:
	printf("RESULT_CONST %f\n", result.value.constant);
	break;
case RESULT_VAR:
	printf("RESULT_VAR\n");
	break;
case RESULT_SUCCESS:
	printf("RESULT_SUCCESS\n");
	break;
case RESULT_ERROR:
	printf("RESULT_ERROR\n");
	break;
default:
	printf("ERROR IN DISASSEMBLING\n");
	}
}
