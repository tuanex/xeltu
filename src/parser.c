#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
case NODE_LEAF:
	if (root->value.type == VALUE_VAR) free(root->value.name);
	__attribute__ ((fallthrough));
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

static Node* expression(void);
static Node* factor(void) {
#ifdef PARSER_DEBUG
	traceStartNode("Factor");
#endif

	Node* node;
	if (accept(TOKEN_IDENTIFIER)) {
		node = makeNode();
		node->type = NODE_LEAF;
		node->value.defined = false;
		node->value.type = VALUE_VAR;
		node->value.name = (char *)malloc(sizeof(char) * (parser.current[-1].length + 1));
		for (int i = 0; i < parser.current[-1].length; i++) {
			node->value.name[i] = parser.current[-1].start[i];
		}
		node->value.name[parser.current[-1].length] = '\0';
	}
	else if (accept(TOKEN_NUMBER)) {
		node = makeNode();
		node->type = NODE_LEAF;
		node->value.defined = true;
		node->value.type = VALUE_CONST;
		node->value.constant = atof(parser.current[-1].start);
	}
	else if (accept(TOKEN_LEFT_PAREN)) {
		node = makeNode();
		node = expression();
		expect(TOKEN_RIGHT_PAREN);
	}
	else {
		node = makeNode();
		node->type = NODE_ERROR;
	}
	
#ifdef PARSER_DEBUG
	traceEndNode("Factor");
#endif
	return node;
}

static Node* term(void) {
#ifdef PARSER_DEBUG
	traceStartNode("Term");
#endif

	Node* node;

	node = factor();

	while (parser.current->type == TOKEN_STAR || 
		parser.current->type == TOKEN_SLASH
	) {
		OpCode op = 
			(parser.current->type == TOKEN_STAR) ? BINOP_MUL : BINOP_DIV;
		nextToken();
		Node* right = factor();
		Node* new_node = makeNode();
		new_node->type = NODE_BINARY;
		new_node->binop_value.op = op;
		new_node->binop_value.left = node;
		new_node->binop_value.right = right;
		node = new_node;
	}

#ifdef PARSER_DEBUG
	traceEndNode("Term");
#endif
	return node;
}

static Node* expression(void) {
#ifdef PARSER_DEBUG
	traceStartNode("Expression");
#endif

	Node* node;

	if (parser.current->type == TOKEN_PLUS || 
		parser.current->type == TOKEN_MINUS
	) {
		UnCode un =
			(parser.current->type == TOKEN_PLUS) ? UNOP_POS: UNOP_NEG;
		nextToken();
		// Make unary
		node = makeNode();
		node->type = NODE_UNARY;
		node->unop_value.un = un;
		node->unop_value.operand = term();
	}
	else {
		node = term();
	}

	while (parser.current->type == TOKEN_PLUS || 
		parser.current->type == TOKEN_MINUS
	) {
		OpCode op = 
			(parser.current->type == TOKEN_PLUS) ? BINOP_ADD : BINOP_SUB;
		nextToken();
		Node* right = term();
		Node* new_node = makeNode();
		new_node->type = NODE_BINARY;
		new_node->binop_value.op = op;
		new_node->binop_value.left = node;
		new_node->binop_value.right = right;
		node = new_node;
	}

#ifdef PARSER_DEBUG
	traceEndNode("Expression");
#endif
	return node;
}

static Node* assignment(void) {
#ifdef PARSER_DEBUG
	traceStartNode("Assignment");
#endif

	Node* node;

	node = expression();

	if (parser.current->type == TOKEN_COLON_EQUAL) {
		OpCode op = BINOP_ASS;
		nextToken();
		Node* right = expression();
		Node* new_node = makeNode();
		new_node->type = NODE_BINARY;
		new_node->binop_value.op = op;
		new_node->binop_value.left = node;
		new_node->binop_value.right = right;
		node = new_node;
	}

#ifdef PARSER_DEBUG
	traceEndNode("Assignment");
#endif
	return node;
}

Node* parse(TokenArray* tokens) {
	initParser(tokens->tokens);
	Node* root = assignment();

#ifdef PARSER_DEBUG
	printf("Disassembling Root Node\n");
	disassembleRootNode(root, 0);
#endif

	return root;
}
