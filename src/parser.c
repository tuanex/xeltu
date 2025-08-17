#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#include "common.h"
#include "parser.h"
#include "scanner.h"

struct Debug {
	int indent;
};
struct Debug debug = {
	.indent = 0,
};
void traceStartNode(const char* msg) {
	debug.indent += 1;
	for (int i = 0; i < debug.indent; i++) {
		printf("-----");
	}
	printf("> %s\n", msg);
}
void traceEndNode(const char* msg) {
	printf("<");
	for (int i = 0; i < debug.indent; i++) {
		printf("-----");
	}
	printf(" %s\n", msg);
	debug.indent -= 1;
}

void freeRootNode(Node* root) {
	switch (root->type) {
case NODE_CONST:
case NODE_UNARY:
case NODE_ERROR:
	free(root);
	return;
case NODE_BINARY:
	freeRootNode(root->binop_value.left);
	freeRootNode(root->binop_value.right);
	free(root);
	}
}

typedef struct Parser {
	TokenArray* tokens;
	Token* current;
	int token;
} Parser;
Parser parser;

static void initParser(Token* token) {
	parser.current = token;
	parser.token = 0;
}

static void nextToken(void) {
	if (parser.current[1].start != NULL)
		parser.current++;
}

static Node* makeNode(void) {
	Node* root = malloc(sizeof(Node));
	return root;
}

static void error(const char* msg) {
	printf("%s\n", msg);
}

static bool accept(TokenType type) {
	if (type == parser.current->type) {
		nextToken();
		return 1;
	}
	return 0;
}

static bool expect(TokenType type) {
	if (accept(type))
		return 1;
	error("error: Unexpected token.");
	return 0;
}

static Node* expression();
static Node* factor(void) {
#ifdef PARSER_DEBUG
	traceStartNode("Factor");
#endif

	Node* result;
	if (accept(TOKEN_NUMBER)) {
		result = makeNode();
		result->type = NODE_CONST;
		result->const_value = atof(parser.current[-1].start);
	}
	else if (accept(TOKEN_LEFT_PAREN)) {
		result = makeNode();
		result = expression();
		expect(TOKEN_RIGHT_PAREN);
	}
	else {
		result = makeNode();
		result->type = NODE_ERROR;
	}
	
#ifdef PARSER_DEBUG
	traceEndNode("Factor");
#endif
	return result;
}

static Node* term(void) {
#ifdef PARSER_DEBUG
	traceStartNode("Term");
#endif

	Node* result;

	result = factor();

	while (parser.current->type == TOKEN_STAR || 
		parser.current->type == TOKEN_SLASH
	) {
		OpCode op = 
			(parser.current->type == TOKEN_STAR) ? BINOP_MUL : BINOP_DIV;
		nextToken();
		Node* right = factor();
		Node* new_result = makeNode();
		new_result->type = NODE_BINARY;
		new_result->binop_value.op = op;
		new_result->binop_value.left = result;
		new_result->binop_value.right = right;
		result = new_result;
	}

#ifdef PARSER_DEBUG
	traceEndNode("Term");
#endif
	return result;
}

static Node* expression(void) {
#ifdef PARSER_DEBUG
	traceStartNode("Expression");
#endif

	Node* result;

	if (parser.current->type == TOKEN_PLUS || 
		parser.current->type == TOKEN_MINUS
	) {
		UnCode un =
			(parser.current->type == TOKEN_PLUS) ? UNOP_POS: UNOP_NEG;
		nextToken();
		// Make unary
		result = makeNode();
		result->type = NODE_UNARY;
		result->unop_value.un = un;
		result->unop_value.operand = term();
	}
	else {
		result = term();
	}

	while (parser.current->type == TOKEN_PLUS || 
		parser.current->type == TOKEN_MINUS
	) {
		OpCode op = 
			(parser.current->type == TOKEN_PLUS) ? BINOP_ADD : BINOP_SUB;
		nextToken();
		Node* right = term();

		Node* new_result = makeNode();
		new_result->type = NODE_BINARY;
		new_result->binop_value.op = op;
		new_result->binop_value.left = result;
		new_result->binop_value.right = right;
		result = new_result;
	}

#ifdef PARSER_DEBUG
	traceEndNode("Expression");
#endif
	return result;
}

Node* parse(TokenArray* tokens) {
	initParser(tokens->tokens);
	Node* root = expression();

#ifdef PARSER_DEBUG
	printf("Disassembling Root Node\n");
	disassembleRootNode(root, 0);
#endif

	return root;
}
