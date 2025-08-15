#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "debug.h"
#include "lexer.h"
#include "parser.h"
#include "value.h"

// DEBUG INFORMATION
#ifdef DEBUG_PARSER_EXECUTION
int indent = 1;
void printStartDebug(const char* msg) {
	for (int i = 0; i < indent; i++) 
		printf("----");
	printf("> %s\n", msg);
	indent++;
}
void printEndDebug(const char* msg) {
	indent--;
	printf("<");
	for (int i = 0; i < indent; i++) 
		printf("----");
	printf(" %s\n", msg);
}
#endif
// END DEBUG INFORMATION


typedef struct {
	TokenArray* list;
	Token* current;
	Token* reading;
	int idx;
} Parser;
Parser parser;

static void initParser(TokenArray* list) {
	parser.list = list;
	parser.current = list->first_token;
	parser.idx = 0;
}

Node errorNode = {
	.tag = ERROR,
};


Value emptyValue = {
	.type = 0,
	.F = 0,
};

Node* makeNode(NodeTag tag, Token token) {
	Node* new_root = malloc(sizeof(Node));
	new_root->tag = tag;
	switch(tag) {
case CONSTANT: {
	char *new_source = NULL;
	new_source = (char*)malloc(token.length + 1);

	if (new_source == NULL) {
		printf("Standard allocation failed.\n");
		return &errorNode;
	}

	memcpy(new_source, token.start, token.length + 1);
	new_source[token.length + 1] = 0;

	new_root->value.leaf_val = makeValue(new_source, TOKEN_NUMBER);
	free(new_source);
	break;
}
default:
	break;
	}
	return new_root;
}

void freeNode(Node* root) {
#ifdef DEBUG_PARSER_EXECUTION
	printf("free node %d\n", root->tag);
#endif
	switch (root->tag) {
case CONSTANT:
	free(root);
	return;
case UNARY:
	free(root);
	return;
case BINARY:
	freeNode(root->value.binop_val.left);
	freeNode(root->value.binop_val.right);
	free(root);
	return;
default:
	return;
	}
}


static void nextToken(void) {
	if (parser.idx < parser.list->count) {
		parser.idx += 1;
		parser.current = &parser.current[1];
	}
}

int accept(TokenType type) {
	if (parser.current->type == type) {
		parser.reading = parser.current;
		nextToken();
		return 1;
	}
	return 0;
}

int expect(TokenType type) {
	if (accept(type)) {
		return 1;
	}
	printf("Unexpected symbol.\n");
	return 0;
}

static Node* expression(void);

static Node* factor(void) {
#ifdef DEBUG_PARSER_EXECUTION
	printStartDebug("Factor");
#endif
	Node* result = &errorNode;
	if (accept(TOKEN_IDENTIFIER)) {
		;
	}
	else if (accept(TOKEN_NUMBER)) {
		result = makeNode(CONSTANT, *parser.reading);
	}
	else if (accept(TOKEN_LEFT_PAREN)) {
		result = expression();
		expect(TOKEN_RIGHT_PAREN);
	}
#ifdef DEBUG_PARSER_EXECUTION
	printEndDebug("Factor");
#endif
	return result;
}

static Node* term(void) {
#ifdef DEBUG_PARSER_EXECUTION
	printStartDebug("Term");
#endif
	if (parser.current->type == TOKEN_STAR || parser.current->type == TOKEN_SLASH) {
		nextToken();
	}
	Node* result = factor();

	while (parser.current->type == TOKEN_STAR || parser.current->type == TOKEN_SLASH) {
		OpCode op;
		if (parser.current->type == TOKEN_STAR) { op = BINOP_MUL; }
		else { op = BINOP_DIV; }
		nextToken();
		Node* right = factor();
		Node* new_root = makeNode(BINARY, *parser.reading);
		new_root->value.binop_val.op = op;
		new_root->value.binop_val.left = result;
		new_root->value.binop_val.right = right;
		result = new_root;
	}
#ifdef DEBUG_PARSER_EXECUTION
	printEndDebug("Term");
#endif
	return result;
}

static Node* expression(void) {
#ifdef DEBUG_PARSER_EXECUTION
	printStartDebug("Expression");
#endif
	if (parser.current->type == TOKEN_PLUS || parser.current->type == TOKEN_MINUS) {
		nextToken();
	}
	Node* result = term();
	while (parser.current->type == TOKEN_PLUS || parser.current->type == TOKEN_MINUS) {
		OpCode op;
		if (parser.current->type == TOKEN_PLUS) { op = BINOP_ADD; }
		else { op = BINOP_SUB; }
		nextToken();
		Node* right = term();
		Node* new_root = makeNode(BINARY, *parser.reading);
		new_root->value.binop_val.op = op;
		new_root->value.binop_val.left = result;
		new_root->value.binop_val.right = right;
		result = new_root;
	}
#ifdef DEBUG_PARSER_EXECUTION
	printEndDebug("Expression");
#endif
	return result;
}

Node* parse(TokenArray* tokens) {

	initParser(tokens);

	Node* root = expression();

#ifdef DEBUG_PARSER_EXECUTION
	disassembleRootNode(root, 0);
#endif

	return root;
}
