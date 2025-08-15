#include <stdio.h>

#include "debug.h"
#include "lexer.h"

void disassembleToken(Token token) {
	switch (token.type) {
case TOKEN_LEFT_PAREN: printf("TOKEN_LEFT_PAREN\n"); return;
case TOKEN_RIGHT_PAREN: printf("TOKEN_RIGHT_PAREN\n"); return;
case TOKEN_LEFT_BRACE: printf("TOKEN_LEFT_BRACE\n"); return;
case TOKEN_RIGHT_BRACE: printf("TOKEN_RIGHT_BRACE\n"); return;

case TOKEN_COMMA: printf("TOKEN_COMMA\n"); return;
case TOKEN_DOT: printf("TOKEN_DOT\n"); return;
case TOKEN_MINUS: printf("TOKEN_MINUS\n"); return;
case TOKEN_PLUS: printf("TOKEN_PLUS\n"); return;
case TOKEN_SEMICOLON: printf("TOKEN_SEMICOLON\n"); return;
case TOKEN_SLASH: printf("TOKEN_SLASH\n"); return;
case TOKEN_STAR: printf("TOKEN_STAR\n"); return;
case TOKEN_BANG: printf("TOKEN_BANG\n"); return;
case TOKEN_BANG_EQUAL: printf("TOKEN_BANG_EQUAL\n"); return;
//case TOKEN_BANG: printf("TOKEN_BANG\n"); return;
case TOKEN_EQUAL: printf("TOKEN_EQUAL\n"); return;
case TOKEN_EQUAL_EQUAL: printf("TOKEN_EQUAL_EQUAL\n"); return;
case TOKEN_GREATER: printf("TOKEN_GREATER\n"); return;
case TOKEN_GREATER_EQUAL: printf("TOKEN_GREATER_EQUAL\n"); return;
case TOKEN_LESS: printf("TOKEN_LESS\n"); return;
case TOKEN_LESS_EQUAL: printf("TOKEN_LESS_EQUAL\n"); return;

case TOKEN_IDENTIFIER: printf("TOKEN_IDENTIFIER "); break;
case TOKEN_NUMBER: printf("TOKEN_NUMBER "); break;

case TOKEN_EOF: printf("TOKEN_EOF\n"); return;
default: 
	printf("Error \n");
	return;
	}

	for (int i = 0; i <= token.length; i++) {
		if (token.start[i] != '\n') printf("%c", token.start[i]);
	}
	printf("\n");
}

void disassembleTokenArray(TokenArray* tokens) {

	for (int i = 0; i < tokens->count; i++) {
		disassembleToken(tokens->first_token[i]);
	}
}

//extern Node errorNode;
void disassembleNode(Value value, int level) {
	for (int i = 0; i < level; i++) {
		printf("	");
	}
	
	switch (value.type) {
case INTEGER:
	printf("integer node: %d\n", value.I);
	break;
case FLOAT:
	printf("float node: %f\n", value.F);
	break;
	}
}

static void disassembleOperation(OpCode op, int level) {
	for (int i = 0; i < level; i++) {
		printf("	");
	}
	switch (op) {
case BINOP_ADD:
	printf("BINOP_ADD");
	break;
case BINOP_SUB:
	printf("BINOP_SUB");
	break;
case BINOP_MUL:
	printf("BINOP_MUL");
	break;
case BINOP_DIV:
	printf("BINOP_DIV");
	break;
	}
	printf("\n");
}

void disassembleRootNode(Node* node, int level) {
	switch (node->tag) {
case CONSTANT:
	disassembleNode(node->value.leaf_val, level);
	break;
case UNARY:
	disassembleNode(node->value.leaf_val, level);
	break;
case BINARY:
	disassembleOperation(node->value.binop_val.op, level);
	disassembleRootNode(node->value.binop_val.left, level + 1);
	disassembleRootNode(node->value.binop_val.right, level + 1);
	break;
case ERROR:
	for (int i = 0; i < level; i++) {
		printf("	");
	}
	printf("ERROR Node\n");

	}
}

//void disassemble
